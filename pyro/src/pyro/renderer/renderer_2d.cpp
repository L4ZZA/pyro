#include "pyro_pch.h"
#include "renderer_2d.h"
#include "vertex_array.h"
#include "shader.h"
#include "render_command.h"
#include <glm/gtc/matrix_transform.hpp>

namespace pyro
{
    struct render_2d_storage
    {
        ref<vertex_array> quad_va;
        ref<shader> texture_shader;
        ref<texture_2d> wite_texture;
    };

    static render_2d_storage *s_data;
}

void pyro::renderer_2d::init()
{
    PYRO_PROFILE_FUNCTION();

    s_data = new render_2d_storage;
    s_data->quad_va = vertex_array::create();

    float rect_vertices[]
    {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    };

    const ref<vertex_buffer> rect_vb = vertex_buffer::create(rect_vertices, sizeof(rect_vertices));

    uint32_t rect_indices[]{0,1,2, 2,3,0};
    const ref<index_buffer> rect_ib = index_buffer::create(rect_indices, sizeof(rect_indices) / sizeof(uint32_t));

    rect_vb->layout({
        {e_shader_data_type::float3, "a_position"},
        {e_shader_data_type::float2, "a_tex_coord"},
    });

    s_data->quad_va = vertex_array::create();
    s_data->quad_va->add_buffer(rect_vb);
    s_data->quad_va->add_buffer(rect_ib);

    texture::wrap(e_texture_wrap::repeat);
    s_data->wite_texture = texture_2d::create(1, 1);
    uint32_t white_tex_data = 0xffffffff; // 2 fs per channel
    s_data->wite_texture->data(&white_tex_data, sizeof(uint32_t));

    s_data->texture_shader = shader::create("assets/shaders/texture_2d.glsl");
    s_data->texture_shader->set_int("u_sampler", 0);
}

void pyro::renderer_2d::shutdown()
{
    PYRO_PROFILE_FUNCTION();
    delete s_data;
}

void pyro::renderer_2d::begin_scene(camera &camera)
{
    PYRO_PROFILE_FUNCTION();
    s_data->texture_shader->bind();
    s_data->texture_shader->set_mat4("u_view_projection", camera.view_projection_matrix());
}

void pyro::renderer_2d::end_scene()
{
    PYRO_PROFILE_FUNCTION();
}

void pyro::renderer_2d::draw_quad(quad_properties const& props)
{
    PYRO_PROFILE_FUNCTION();
    
    s_data->texture_shader->set_float4("u_color", props.color); 
    s_data->texture_shader->set_float("u_tiling_factor", props.tiling_factor); 
    if(props.texture)
        props.texture->bind(); 
    else
        s_data->wite_texture->bind();
    // REMEMBER TRS -> in reverse for opengl SCALE, ROTATE, TRANSLATE. 
    // https://gamedev.stackexchange.com/a/16721/129225
    glm::mat4 transform(1); 
    transform = glm::scale(transform, {props.size.x, props.size.y, 1.0f}); 
    transform = glm::rotate(transform, props.rotation, {0.0f, 0.0f, 1.0f}); 
    transform = glm::translate(transform, props.position); 
    s_data->texture_shader->set_mat4("u_transform", transform); 
 
    s_data->quad_va->bind(); 
    render_command::draw_indexed(s_data->quad_va); 
}

void pyro::renderer_2d::draw_rotated_quad(quad_properties const& props)
{
    PYRO_PROFILE_FUNCTION();
    
    s_data->texture_shader->set_float4("u_color", props.color); 
    s_data->texture_shader->set_float("u_tiling_factor", props.tiling_factor); 
    if(props.texture)
        props.texture->bind(); 
    else
        s_data->wite_texture->bind();
    // REMEMBER TRS -> in reverse for opengl SCALE, ROTATE, TRANSLATE. 
    // https://gamedev.stackexchange.com/a/16721/129225
    glm::mat4 transform(1); 
    transform = glm::scale(transform, {props.size.x, props.size.y, 1.0f}); 
    transform = glm::translate(transform, props.position); 
    s_data->texture_shader->set_mat4("u_transform", transform); 
 
    s_data->quad_va->bind(); 
    render_command::draw_indexed(s_data->quad_va); 
}
