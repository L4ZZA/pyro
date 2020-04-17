/*
* Simple flat color shader, with dynamic update of the color.
*/

#type vertex
#version 430

layout(location = 0) in vec3 a_position; 

uniform mat4 u_view_projection; 
uniform mat4 u_transform; 

out vec3 v_position; 

void main() 
{ 
    v_position = a_position; 
    gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0); 
} 

#type fragment
#version 430

layout(location = 0) out vec4 o_color; 

in vec3 v_position; 

uniform vec4 u_color; 

void main() 
{ 
    o_color = u_color; 
}
