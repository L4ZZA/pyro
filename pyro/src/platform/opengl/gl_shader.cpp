#include "pyro_pch.h"
#include "gl_shader.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include <fstream>

static uint32_t shader_type_from_string(const std::string& type)
{
    if(type == "vertex")
        return GL_VERTEX_SHADER;
    if(type == "fragment" || type == "pixel")
        return GL_FRAGMENT_SHADER;

    PYRO_CORE_ASSERT(false, "[gl_shader] Unknown shader type!");
    return 0;
}

static std::string shader_type_to_string(uint32_t type)
{
    if(type == GL_VERTEX_SHADER)
        return "vertex";
    if(type == GL_FRAGMENT_SHADER)
        return "fragment/pixel";

    PYRO_CORE_ASSERT(false, "[gl_shader] Unknown shader type!");
    return 0;
}

pyro::gl_shader::gl_shader(const std::string& file_path)
    :m_file_path(file_path)
{
    const std::string source  = read_file(file_path);
    const auto shader_sources = pre_process(source);
    // Extract name from file_path
    m_name = extract_name(file_path);
    compile(shader_sources);
}

pyro::gl_shader::gl_shader(const std::string& name, const std::string& vertex_source, const std::string& fragment_source)
    : m_name(name)
{
    std::unordered_map<uint32_t, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertex_source;
    sources[GL_FRAGMENT_SHADER] = fragment_source;
    compile(sources);
}

pyro::gl_shader::~gl_shader()
{
    PYRO_CORE_TRACE("[gl_shader] Deleteing shader program {} - id: {}", m_name, m_program_id);
    glDeleteProgram(m_program_id);
}


std::string pyro::gl_shader::read_file(const std::string& file_path)
{
    std::string result;
    std::ifstream in(file_path, std::ios::in, std::ios::binary);
    if(in)
    {
        in.seekg(0, std::ios::end);
		size_t size = in.tellg();
		if (size != -1)
		{
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
		}
        else
        {
            PYRO_CORE_ERROR("[read_file] Could not read from file '{}'", file_path);
        }
    }
    else
    {
        PYRO_CORE_ERROR("[read_file] Could not open file '{}'", file_path);
    }
    return result;
}

std::unordered_map<uint32_t, std::string> pyro::gl_shader::pre_process(const std::string& source)
{
    std::unordered_map<uint32_t, std::string> sources;
    const char* type_token = "#type";
    const size_t type_token_length = strlen(type_token);
    size_t pos = source.find(type_token, 0); //Start of shader type declaration line
    while(pos != std::string::npos)
    {
        size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
        PYRO_CORE_ASSERT(eol != std::string::npos, "[gl_shader] Syntax error");
        size_t begin = pos + type_token_length + 1; //Start of shader type name (after "#type " keyword)
        std::string type = source.substr(begin, eol - begin);
        PYRO_CORE_ASSERT(shader_type_from_string(type), "[gl_shader] Invalid shader type specified");

        size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
        PYRO_CORE_ASSERT(nextLinePos != std::string::npos, "[gl_shader] Syntax error");
		pos = source.find(type_token, nextLinePos); //Start of next shader type declaration line

        sources[shader_type_from_string(type)] = (pos == std::string::npos) ? 
            source.substr(nextLinePos) :
            source.substr(nextLinePos, pos - nextLinePos);
    }

    return sources;
}

std::string pyro::gl_shader::extract_name(const std::string& file_path)
{
    auto last_slash = file_path.find_last_of("/\\");
    last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;
    const auto last_dot = file_path.rfind('.');
    const auto count = last_dot == std::string::npos ? file_path.size() - last_slash : last_dot - last_slash;
    return file_path.substr(last_slash, count);
}

void pyro::gl_shader::compile(const std::unordered_map<uint32_t, std::string>& sources)
{
    uint32_t program = glCreateProgram();
    PYRO_CORE_TRACE("[gl_shader] Creating shader {} - id: {}", m_name, program);
    std::vector<uint32_t> shader_ids(sources.size());

    for(auto& [type, source] : sources)
    {
        uint32_t shader = glCreateShader(type);

        const char* source_cstr = source.c_str();
        glShaderSource(shader, 1, &source_cstr, 0);

        PYRO_CORE_INFO("[gl_shader] Compiling shader: {}", m_file_path);
        glCompileShader(shader);

        int32_t is_compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
        if(is_compiled == GL_FALSE)
        {
            int32_t max_length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

            std::vector<char> info_log(max_length);
            glGetShaderInfoLog(shader, max_length, &max_length, &info_log[0]);

            glDeleteShader(shader);

            PYRO_CORE_ERROR("[gl_shader] {0}", info_log.data());
            PYRO_CORE_ASSERT(false, "[gl_shader] Compilation failed!");
            break;
        }

        glAttachShader(program, shader);
        shader_ids.push_back(shader);
    }

    m_program_id = program;

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    int32_t is_linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
    if(is_linked == GL_FALSE)
    {
        int32_t max_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

        // The maxLength includes the NULL character
        std::vector<char> info_log(max_length);
        glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);

        for(auto id : shader_ids)
            glDeleteShader(id);

        PYRO_CORE_ERROR("[gl_shader] {0}", info_log.data());
        PYRO_CORE_ASSERT(false, "[gl_shader] Shader link failure!");
        return;
    }

    for(auto id : shader_ids)
    {
        glDetachShader(program, id);
        PYRO_CORE_INFO("[gl_shader] Deleted shader id:{} - '{}'", id, m_file_path);
        glDeleteShader(id);
    }
}

int32_t pyro::gl_shader::get_uniform_location(const std::string& name) const
{
    if(m_uniform_cache.find(name) != m_uniform_cache.end())
    {
        return m_uniform_cache[name];
    }

    const int32_t location = glGetUniformLocation(m_program_id, name.c_str());
    if(location < 0)
    {
        PYRO_CORE_WARN("[gl_shader] Uniform '{}' not found in shader {}", name, m_program_id);
    }
    else
    {
        m_uniform_cache[name] = location;
    }
    return location;
}

void pyro::gl_shader::bind() const
{
    glUseProgram(m_program_id);
}

void pyro::gl_shader::unbind() const
{
    glUseProgram(0);
}

const std::string& pyro::gl_shader::name() const
{
    return m_name;
}

void pyro::gl_shader::set_int(const std::string& name, int32_t val)
{
    upload_uniform(name, val);
}

void pyro::gl_shader::set_float(const std::string& name, float val)
{
    upload_uniform(name, val);
}

void pyro::gl_shader::set_vec2(const std::string& name, const glm::vec2& vec)
{
    upload_uniform(name, vec);
}

void pyro::gl_shader::set_vec3(const std::string& name, const glm::vec3& vec)
{
    upload_uniform(name, vec);
}

void pyro::gl_shader::set_vec4(const std::string& name, const glm::vec4& vec)
{
    upload_uniform(name, vec);
}

void pyro::gl_shader::set_mat4(const std::string& name, const glm::mat4& mat)
{
    upload_uniform(name, mat);
}

void pyro::gl_shader::upload_uniform(const std::string& name, int32_t val) const
{
    const int32_t uniformLocation = get_uniform_location(name);
    glUniform1i(uniformLocation, val);

    //PYRO_CORE_TRACE("[shader] upload_uniform (float) (prog {0}): uniform: '{1}' = {2}(float)", m_program_id, name, val); 
}

void pyro::gl_shader::upload_uniform(const std::string& name, float val) const
{
    const int32_t uniformLocation = get_uniform_location(name);
    glUniform1f(uniformLocation, val);

    //PYRO_CORE_TRACE("[shader] upload_uniform (float) (prog {0}): uniform: '{1}' = {2}(float)", m_program_id, name, val); 
}

void pyro::gl_shader::upload_uniform(const std::string& name, const glm::vec2& vec) const
{
    const int32_t uniformLocation = get_uniform_location(name);
    glUniform2f(uniformLocation, vec.x, vec.y);

    //PYRO_CORE_TRACE("[shader] upload_uniform (float) (prog {0}): uniform: '{1}' = {2}(float)", m_program_id, name, vec); 
}

void pyro::gl_shader::upload_uniform(const std::string& name, const glm::vec3& vec) const
{
    const int32_t uniformLocation = get_uniform_location(name);
    glUniform3f(uniformLocation, vec.x, vec.y, vec.z);

    //PYRO_CORE_TRACE("[shader] upload_uniform (float) (prog {0}): uniform: '{1}' = {2}(float)", m_program_id, name, vec); 
}

void pyro::gl_shader::upload_uniform(const std::string& name, const glm::vec4& vec) const
{
    const int32_t uniformLocation = get_uniform_location(name);
    glUniform4f(uniformLocation, vec.x, vec.y, vec.z, vec.w);

    //PYRO_CORE_TRACE("[shader] upload_uniform (float) (prog {0}): uniform: '{1}' = {2}(float)", m_program_id, name, vec); 
}

void pyro::gl_shader::upload_uniform(const std::string& name, const glm::mat4& mat) const
{
    const int32_t uniformLocation = get_uniform_location(name);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat));

    //PYRO_CORE_TRACE("[shader] upload_uniform (glm::mat4) (prog {0}): uniform: '{1}' = {2}(mat4)", m_program_id, name, mat); 
}



