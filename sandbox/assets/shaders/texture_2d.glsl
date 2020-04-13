/*
* Simple texture shader.
*/

#type vertex
#version 430

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_tex_coord;

uniform mat4 u_view_projection;

out vec4 v_color;
out vec2 v_tex_coord;

void main()
{
	v_color = a_color;
    v_tex_coord = a_tex_coord;
    gl_Position = u_view_projection * vec4(a_position, 1.0);
}

#type fragment
#version 430

layout(location = 0) out vec4 o_color;

in vec4 v_color;
in vec2 v_tex_coord;

uniform vec4 u_color;  
uniform float u_tiling_factor;  
uniform sampler2D u_sampler; 

float vignette(vec2 screen_pos, float radius)
{
	float distance = 1.0f - distance(screen_pos * radius, vec2(0.0f));
	distance = clamp(distance, 0.f, 1.f);
	distance = sqrt(distance);
	return distance;
}

void main() 
{ 
    //o_color = texture(u_sampler, v_tex_coord * u_tiling_factor) * u_color; 
    o_color = v_color;
}
