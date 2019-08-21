#include "pyro_pch.h"
#include "render_command.h"

#include "platform/opengl/gl_renderer_api.h"

pyro::renderer_api* pyro::render_command::s_renderer_api = new pyro::gl_renderer_api;
