#include "pyro_pch.h"
#include "shader.h"
#include "renderer.h"
#include "platform/opengl/gl_shader.h"

pyro::ref<pyro::shader> pyro::shader::create(const std::string& file_path)
{
    switch (renderer::api())
    {
        case renderer_api::e_api::none: PYRO_CORE_ASSERT(false, "[shader] e_renderer_api::none currently not supported!"); return nullptr;
        case renderer_api::e_api::open_gl: return std::make_shared<gl_shader>(file_path);
    }

    PYRO_CORE_ASSERT(false, "[shader] Unknown renderer api!");
    return nullptr;
}

pyro::ref<pyro::shader> pyro::shader::create(const std::string& name, const std::string& vertex_source, const std::string& fragment_source)
{
    switch (renderer::api())
    {
        case renderer_api::e_api::none: PYRO_CORE_ASSERT(false, "[shader] e_renderer_api::none currently not supported!"); return nullptr;
        case renderer_api::e_api::open_gl: return std::make_shared<gl_shader>(name, vertex_source, fragment_source);
    }

    PYRO_CORE_ASSERT(false, "[shader] Unknown renderer api!");
    return nullptr;
}

//------------------------------------------------------------------------------------------------

void pyro::shader_library::add(const ref<shader>& shader)
{
    std::string name = shader->name();
    if(!exists(name))
    {
        m_shaders[name] = shader;
    }
}

pyro::ref<pyro::shader> pyro::shader_library::load(const std::string& filepath)
{
    ref<shader> shader = shader::create(filepath);
    add(shader);
    return shader;
}

pyro::ref<pyro::shader> pyro::shader_library::load(const std::string& name, const std::string& filepath)
{
    ref<shader> shader = shader::create(filepath);
    add(shader);
    return shader;
}

pyro::ref<pyro::shader> pyro::shader_library::get(const std::string& name)
{
    PYRO_CORE_ASSERT(exists(name), "[shader_library] shader not found!");
    return m_shaders[name];
}

bool pyro::shader_library::exists(const std::string& name)
{
    return m_shaders.find(name) != m_shaders.end();
}
