#include "pyro_pch.h"
#include "vertex_array.h"
#include "renderer.h"

#include "platform/opengl/gl_vertex_array.h"

pyro::ref<pyro::vertex_array> pyro::vertex_array::create()
{
    switch (renderer::api())
    {
        case renderer_api::e_api::none: PYRO_CORE_ASSERT(false, "[vertex_array] e_renderer_api::none currently not supported!"); return nullptr;
        case renderer_api::e_api::opengl: return make_ref<gl_vertex_array>();
    }

    PYRO_CORE_ASSERT(false, "[vertex_array] Unknown renderer api!");
    return nullptr;
}
