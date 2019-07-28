#include "pyro_pch.h"
#include "gl_shader.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"


pyro::gl_shader::gl_shader(const std::string& vertex_source, const std::string& fragment_source)
{
    // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar* source = vertex_source.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        PYRO_CORE_ERROR("{}", infoLog.data());
        PYRO_ASSERT(false, "Vertex shader compilation failed!");
        return;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = fragment_source.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        PYRO_CORE_ERROR("{}", infoLog.data());
        PYRO_ASSERT(false, "Fragment shader compilation failed!");
        return;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    GLuint program = glCreateProgram();
    m_program_id = program;

    // Attach our shaders to our program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)& isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        PYRO_CORE_ERROR("{}", infoLog.data());
        PYRO_ASSERT(false, "Shader program failed to compile!");

        // In this simple program, we'll just leave
        return;
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
}

pyro::gl_shader::~gl_shader()
{
    glDeleteProgram(m_program_id);
}


void pyro::gl_shader::bind() const
{
    glUseProgram(m_program_id);
}

void pyro::gl_shader::unbind() const
{
    glUseProgram(0);
}

void pyro::gl_shader::set_uniform(const std::string& name, int32_t val) 
{ 
    const int32_t uniformLocation = glGetUniformLocation(m_program_id, name.c_str()); 
    glUniform1i(uniformLocation, val); 

    //PYRO_CORE_TRACE("[shader] set_uniform (float) (prog {0}): uniform: '{1}' = {2}(float)", m_program_id, name, val); 
} 

void pyro::gl_shader::set_uniform(const std::string& name, float val) 
{ 
    const int32_t uniformLocation = glGetUniformLocation(m_program_id, name.c_str()); 
    glUniform1f(uniformLocation, val); 

    //PYRO_CORE_TRACE("[shader] set_uniform (float) (prog {0}): uniform: '{1}' = {2}(float)", m_program_id, name, val); 
} 

void pyro::gl_shader::set_uniform(const std::string& name, const glm::vec2& vec) 
{
    const int32_t uniformLocation = glGetUniformLocation(m_program_id, name.c_str());
    glUniform2f(uniformLocation, vec.x, vec.y);

    //PYRO_CORE_TRACE("[shader] set_uniform (float) (prog {0}): uniform: '{1}' = {2}(float)", m_program_id, name, vec); 
} 

void pyro::gl_shader::set_uniform(const std::string& name, const glm::vec3& vec) 
{
    const int32_t uniformLocation = glGetUniformLocation(m_program_id, name.c_str());
    glUniform3f(uniformLocation, vec.x, vec.y, vec.z);

    //PYRO_CORE_TRACE("[shader] set_uniform (float) (prog {0}): uniform: '{1}' = {2}(float)", m_program_id, name, vec); 
} 

void pyro::gl_shader::set_uniform(const std::string& name, const glm::vec4& vec) 
{
    const int32_t uniformLocation = glGetUniformLocation(m_program_id, name.c_str());
    glUniform4f(uniformLocation, vec.x, vec.y, vec.z, vec.w);

    //PYRO_CORE_TRACE("[shader] set_uniform (float) (prog {0}): uniform: '{1}' = {2}(float)", m_program_id, name, vec); 
} 

void pyro::gl_shader::set_uniform(const std::string& name, const glm::mat4& mat) 
{
    const int32_t uniformLocation = glGetUniformLocation(m_program_id, name.c_str());
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat));

    //PYRO_CORE_TRACE("[shader] set_uniform (glm::mat4) (prog {0}): uniform: '{1}' = {2}(mat4)", m_program_id, name, mat); 
} 

