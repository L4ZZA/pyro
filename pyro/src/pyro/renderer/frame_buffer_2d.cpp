#include "pyro_pch.h"
#include "pyro/renderer/frame_buffer_2d.h"
#include "pyro/renderer/renderer.h"
#include "platform/opengl/gl_frame_buffer_2d.h"

std::shared_ptr<pyro::frame_buffer_2d>
pyro::frame_buffer_2d::create(framebuffer_props properties)
{
    switch(renderer::api())
    {
        case renderer_api::e_api::none: PYRO_CORE_ASSERT(false, "[frame_buffer_2d] e_renderer_api::none currently not supported!"); return nullptr;
        case renderer_api::e_api::opengl: return make_ref<gl_frame_buffer_2d>(properties);
    }

    PYRO_CORE_ASSERT(false, "[frame_buffer_2d] Unknown renderer api!");
    return nullptr;
}
