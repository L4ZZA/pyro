#pragma once
#include <string>


static const std::string vertex_shader_3d = R"(
    #version 430

    layout(location = 0) in vec3 a_position;
    layout(location = 1) in vec3 a_normal;
    layout(location = 2) in vec2 a_tex_coord;

    uniform mat4 u_view_projection;
    uniform mat4 u_transform;

    out vec3 v_position;
    out vec2 v_tex_coord; 

    void main()
    {
        v_position = a_position;
        v_tex_coord = a_tex_coord;
        gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
    }
)";

static const std::string fragment_shader_3d = R"(
    #version 430

    layout(location = 0) out vec4 o_color;

    in vec3 v_position;
    in vec2 v_tex_coord;

    uniform sampler2D u_sampler; 
    uniform bool u_with_texture = true; 
    uniform vec4 u_color = vec4(0.2f, 0.3f, 0.8f, 1.f); 

    void main()
    {
        if(u_with_texture)
            o_color = texture(u_sampler, v_tex_coord); 
        else
            o_color = u_color;
    }
)";