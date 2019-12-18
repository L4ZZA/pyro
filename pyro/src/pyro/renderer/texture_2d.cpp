#include "pyro_pch.h"
#include "pyro/renderer/texture_2d.h"

#include "pyro/renderer/renderer.h"
#include "platform/opengl/gl_texture.h"


std::shared_ptr<pyro::texture_2d> pyro::texture_2d::create(uint32_t width, uint32_t height)
{
    switch(renderer::api())
    {
        case renderer_api::e_api::none: PYRO_CORE_ASSERT(false, "[texture_2d] e_renderer_api::none currently not supported!"); return nullptr;
        case renderer_api::e_api::opengl: return make_ref<gl_texture_2d>(width, height);
    }

    PYRO_CORE_ASSERT(false, "[texture_2d] Unknown renderer api!");
    return nullptr;
}

std::shared_ptr<pyro::texture_2d> pyro::texture_2d::create(std::string const &path)
{
    switch(renderer::api())
    {
        case renderer_api::e_api::none: PYRO_CORE_ASSERT(false, "[texture_2d] e_renderer_api::none currently not supported!"); return nullptr;
        case renderer_api::e_api::opengl: return make_ref<gl_texture_2d>(path);
    }

    PYRO_CORE_ASSERT(false, "[texture_2d] Unknown renderer api!");
    return nullptr;
}