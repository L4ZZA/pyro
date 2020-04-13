#include "pyro_pch.h"
#include "renderer_2d.h"
#include "vertex_array.h"
#include "shader.h"
#include "render_command.h"
#include <glm/gtc/matrix_transform.hpp>

namespace pyro
{
    struct quad_vertex 
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 tex_coord;
        // TODO:  textId
    };

    static const uint32_t s_quad_vertices = 4;
    static const uint32_t s_quad_indices  = 6;
    struct render_2d_data
    {
        const uint32_t max_quads     = 10000;
        const uint32_t max_vertices  = max_quads * s_quad_vertices;
        const uint32_t max_indices   = max_quads * s_quad_indices;

        ref<vertex_array> quad_vertex_array;
        ref<vertex_buffer> quad_vertex_buffer;
        ref<shader> texture_shader;
        ref<texture_2d> wite_texture;

        uint32_t quad_index_count = 0;
        quad_vertex *quad_vertex_buffer_base = nullptr;
        quad_vertex *quad_vertex_buffer_ptr  = nullptr;
    };

    static render_2d_data s_data;
}

void pyro::renderer_2d::init()
{
    PYRO_PROFILE_FUNCTION();

    s_data.quad_vertex_array = vertex_array::create();

    s_data.quad_vertex_buffer = vertex_buffer::create(s_data.max_vertices * sizeof(quad_vertex));
    s_data.quad_vertex_buffer->layout({
        {e_shader_data_type::float3, "a_position"},
        {e_shader_data_type::float4, "a_color"},
        {e_shader_data_type::float2, "a_tex_coord"},
    });
    s_data.quad_vertex_array->add_buffer(s_data.quad_vertex_buffer);

    s_data.quad_vertex_buffer_base = new quad_vertex[s_data.max_vertices];

    uint32_t *quad_indices = new uint32_t[s_data.max_indices];
    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_data.max_indices; i += s_quad_indices)
    {
        quad_indices[i + 0] = offset + 0;
        quad_indices[i + 1] = offset + 1;
        quad_indices[i + 2] = offset + 2;

        quad_indices[i + 3] = offset + 2;
        quad_indices[i + 4] = offset + 3;
        quad_indices[i + 5] = offset + 0;

        offset += s_quad_vertices;
    }


    const ref<index_buffer> quad_ib = index_buffer::create(quad_indices, s_data.max_indices);
    s_data.quad_vertex_array->add_buffer(quad_ib);
    delete[] quad_indices;

    texture::wrap(e_texture_wrap::repeat);
    s_data.wite_texture = texture_2d::create(1, 1);
    uint32_t white_tex_data = 0xffffffff; // 2 Fs per channel
    s_data.wite_texture->data(&white_tex_data, sizeof(uint32_t));

    s_data.texture_shader = shader::create("assets/shaders/texture_2d.glsl");
    //s_data.texture_shader->set_int("u_sampler", 0);
}

void pyro::renderer_2d::shutdown()
{
    PYRO_PROFILE_FUNCTION();
}

void pyro::renderer_2d::begin_scene(camera &camera)
{
    PYRO_PROFILE_FUNCTION();
    s_data.texture_shader->bind();
    s_data.texture_shader->set_mat4("u_view_projection", camera.view_projection_matrix());

    s_data.quad_index_count = 0;
    s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;
}

void pyro::renderer_2d::end_scene()
{
    PYRO_PROFILE_FUNCTION();
    
    uint32_t data_size = (uint8_t*)s_data.quad_vertex_buffer_ptr - (uint8_t*)s_data.quad_vertex_buffer_base;
    s_data.quad_vertex_buffer->data(s_data.quad_vertex_buffer_base, data_size);

    flush();
}

void pyro::renderer_2d::flush()
{
    render_command::draw_indexed(s_data.quad_vertex_array, s_data.quad_index_count);
}

void pyro::renderer_2d::draw_quad(quad_properties const& props)
{
    PYRO_PROFILE_FUNCTION();
    
    s_data.quad_vertex_buffer_ptr->position = props.position;
    s_data.quad_vertex_buffer_ptr->color = props.color;
    s_data.quad_vertex_buffer_ptr->tex_coord = { 0.f, 0.f };
    s_data.quad_vertex_buffer_ptr++;
    
    s_data.quad_vertex_buffer_ptr->position = { props.position.x + props.size.x, props.position.y, 0.f };
    s_data.quad_vertex_buffer_ptr->color = props.color;
    s_data.quad_vertex_buffer_ptr->tex_coord = { 1.f, 0.f };
    s_data.quad_vertex_buffer_ptr++;
    
    s_data.quad_vertex_buffer_ptr->position = { props.position.x + props.size.x, props.position.y + props.size.y, 0.f };
    s_data.quad_vertex_buffer_ptr->color = props.color;
    s_data.quad_vertex_buffer_ptr->tex_coord = { 1.f, 1.f };
    s_data.quad_vertex_buffer_ptr++;
    
    s_data.quad_vertex_buffer_ptr->position = { props.position.x, props.position.y + props.size.y, 0.f };
    s_data.quad_vertex_buffer_ptr->color = props.color;
    s_data.quad_vertex_buffer_ptr->tex_coord = { 0.f, 1.f };
    s_data.quad_vertex_buffer_ptr++;

    s_data.quad_index_count += s_quad_indices; // 6 indices per quad

    //s_data.texture_shader->set_float4("u_color", props.color); 
    //s_data.texture_shader->set_float("u_tiling_factor", props.tiling_factor); 
    //if(props.texture)
    //    props.texture->bind(); 
    //else
    //    s_data.wite_texture->bind();
    //// REMEMBER TRS -> in reverse for opengl SCALE, ROTATE, TRANSLATE. 
    //// https://gamedev.stackexchange.com/a/16721/129225
    //glm::mat4 transform(1); 
    //transform = glm::scale(transform, {props.size.x, props.size.y, 1.0f}); 
    //transform = glm::rotate(transform, props.rotation, {0.0f, 0.0f, 1.0f}); 
    //transform = glm::translate(transform, props.position); 
    //s_data.texture_shader->set_mat4("u_transform", transform); 
 
    //s_data.quad_va->bind(); 
    //render_command::draw_indexed(s_data.quad_va); 
}

void pyro::renderer_2d::draw_rotated_quad(quad_properties const& props)
{
    PYRO_PROFILE_FUNCTION();
    
    s_data.texture_shader->set_float4("u_color", props.color); 
    s_data.texture_shader->set_float("u_tiling_factor", props.tiling_factor); 
    if(props.texture)
        props.texture->bind(); 
    else
        s_data.wite_texture->bind();
    // REMEMBER TRS -> in reverse for opengl SCALE, ROTATE, TRANSLATE. 
    // https://gamedev.stackexchange.com/a/16721/129225
    glm::mat4 transform(1); 
    transform = glm::scale(transform, {props.size.x, props.size.y, 1.0f}); 
    transform = glm::translate(transform, props.position); 
    s_data.texture_shader->set_mat4("u_transform", transform); 
 
    s_data.quad_vertex_array->bind(); 
    render_command::draw_indexed(s_data.quad_vertex_array); 
}
