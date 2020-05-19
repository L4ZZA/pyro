#include "pyro_pch.h"
#include "pyro/renderer/texture.h"

#include "pyro/renderer/renderer.h"
#include "platform/opengl/gl_texture.h"


pyro::ref<pyro::texture_2d>
pyro::texture_2d::create(
    uint32_t width,
    uint32_t height,
    texture_parameters const &params /*= texture_parameters()*/)
{
    switch(renderer::api())
    {
        case renderer_api::e_api::none: PYRO_CORE_ASSERT(false, "[texture_2d] e_renderer_api::none currently not supported!"); return nullptr;
        case renderer_api::e_api::opengl: return make_ref<gl_texture_2d>(width, height, params);
    }

    PYRO_CORE_ASSERT(false, "[texture_2d] Unknown renderer api!");
    return nullptr;
}

std::shared_ptr<pyro::texture_2d>
pyro::texture_2d::create_from_file(
    std::string const &path,
    texture_parameters const &params /*= texture_parameters()*/)
{
    switch(renderer::api())
    {
        case renderer_api::e_api::none: PYRO_CORE_ASSERT(false, "[texture_2d] e_renderer_api::none currently not supported!"); return nullptr;
        case renderer_api::e_api::opengl: return make_ref<gl_texture_2d>(path, params);
    }

    PYRO_CORE_ASSERT(false, "[texture_2d] Unknown renderer api!");
    return nullptr;
}