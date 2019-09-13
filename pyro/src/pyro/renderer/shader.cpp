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

pyro::ref<pyro::shader> pyro::shader::create(const std::string& vertex_source, const std::string& fragment_source)
{
    switch (renderer::api())
    {
        case renderer_api::e_api::none: PYRO_CORE_ASSERT(false, "[shader] e_renderer_api::none currently not supported!"); return nullptr;
        case renderer_api::e_api::open_gl: return std::make_shared<gl_shader>(vertex_source, fragment_source);
    }

    PYRO_CORE_ASSERT(false, "[shader] Unknown renderer api!");
    return nullptr;
}
