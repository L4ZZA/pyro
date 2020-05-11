#include "pyro_pch.h"
#include "gl_renderer_api.h"
#include "glad/glad.h"

enum class pyro::e_primitive_type 
{ 
    points = GL_POINTS, 
    lines = GL_LINES, 
    line_loop = GL_LINE_LOOP, 
    line_strip = GL_LINE_STRIP, 
    triangles = GL_TRIANGLES, 
    triangle_strip = GL_TRIANGLE_STRIP, 
    triangle_fan = GL_TRIANGLE_FAN, 
};

pyro::e_primitive_type pyro::renderer_api::s_primitive_type = pyro::e_primitive_type::triangles; 

void pyro::gl_renderer_api::init()
{
	PYRO_PROFILE_FUNCTION();
    enable_alpha();
    enable_depth_test();
}

void pyro::gl_renderer_api::enable_wireframe()
{
    glPolygonMode(GL_FRONT, GL_LINE); 
    glPolygonMode(GL_BACK, GL_LINE); 
}

void pyro::gl_renderer_api::disable_wireframe()
{
    glPolygonMode(GL_FRONT, GL_FILL); 
    glPolygonMode(GL_BACK, GL_FILL); 
}

void pyro::gl_renderer_api::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void pyro::gl_renderer_api::clear_color(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void pyro::gl_renderer_api::resize_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height); 
}

void pyro::gl_renderer_api::draw_indexed(const ref<vertex_array>& vertex_array, const uint32_t index_count /*= 0*/)
{
    uint32_t count = index_count ? index_count : vertex_array->index_buffer()->count();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void pyro::gl_renderer_api::primitive_type(const e_primitive_type& type)
{
    s_primitive_type = type; 
}

//------------------------------------------------------------------------------------------------- 

void pyro::gl_renderer_api::enable_alpha()
{
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}

void pyro::gl_renderer_api::enable_depth_test()
{
    glEnable(GL_DEPTH_TEST); 
}

void pyro::gl_renderer_api::enable_culling()
{
    glEnable(GL_CULL_FACE); 
    glCullFace(GL_BACK); 
}
