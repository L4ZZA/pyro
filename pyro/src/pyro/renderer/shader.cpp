#include "pyro_pch.h"
#include "shader.h"
#include "renderer.h"
#include "platform/opengl/gl_shader.h"

pyro::ref<pyro::shader> pyro::shader::create(std::string const &file_path)
{
    switch(renderer::api())
    {
        case renderer_api::e_api::none: PYRO_CORE_ASSERT(false, "[shader] e_renderer_api::none currently not supported!"); return nullptr;
        case renderer_api::e_api::open_gl: return make_ref<gl_shader>(file_path);
    }

    PYRO_CORE_ASSERT(false, "[shader] Unknown renderer api!");
    return nullptr;
}

pyro::ref<pyro::shader> pyro::shader::create(std::string const &name, std::string const &vertex_source, std::string const &fragment_source)
{
    switch(renderer::api())
    {
        case renderer_api::e_api::none: PYRO_CORE_ASSERT(false, "[shader] e_renderer_api::none currently not supported!"); return nullptr;
        case renderer_api::e_api::open_gl: return make_ref<gl_shader>(name, vertex_source, fragment_source);
    }

    PYRO_CORE_ASSERT(false, "[shader] Unknown renderer api!");
    return nullptr;
}

//------------------------------------------------------------------------------------------------

void pyro::shader_library::add(const ref<shader> &shader)
{
    std::string name = shader->name();
    if(!exists(name))
    {
        m_shaders[name] = shader;
    }
    else
    {
        PYRO_CORE_ERROR("[shader_library] {} shader already exists!", name);
    }
}

pyro::ref<pyro::shader> pyro::shader_library::load(std::string const &filepath)
{
    ref<shader> shader = shader::create(filepath);
    add(shader);
    return get(shader->name());
}

pyro::ref<pyro::shader> pyro::shader_library::load(std::string const &name, std::string const &filepath)
{
    ref<shader> shader = shader::create(name, filepath);
    add(shader);
    return get(name);
}

pyro::ref<pyro::shader> pyro::shader_library::get(std::string const &name)
{
    PYRO_CORE_ASSERT(exists(name), "[shader_library] shader not found!");
    return m_shaders[name];
}

bool pyro::shader_library::exists(std::string const &name)
{
    return m_shaders.find(name) != m_shaders.end();
}
