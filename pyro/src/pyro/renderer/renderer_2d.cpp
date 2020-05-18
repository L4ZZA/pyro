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
        float tex_index;
        float tiling_factor;
    };

    struct render_2d_data
    {
        ref<vertex_array> quad_vertex_array;
        ref<vertex_buffer> quad_vertex_buffer;
        ref<shader> texture_shader;
        ref<texture_2d> wite_texture;

        uint32_t quad_index_count = 0;
        quad_vertex *quad_vertex_buffer_base = nullptr;
        quad_vertex *quad_vertex_buffer_ptr  = nullptr;

        std::array<ref<texture_2d>, renderer_2d::max_texture_slots> texture_slots;
        // Index of the next empty texture slot
        // 0 white texture index [default]
        uint32_t texture_slot_index = 1; 

        glm::vec4 quad_vertex_positions[4];

        renderer_2d::statistics stats;
    };

    static render_2d_data s_data;
}

void pyro::renderer_2d::init()
{
    PYRO_PROFILE_FUNCTION();

    s_data.quad_vertex_array = vertex_array::create();

    s_data.quad_vertex_buffer = 
        vertex_buffer::create(renderer_2d::max_vertices * sizeof(quad_vertex));
    s_data.quad_vertex_buffer->layout({
        {e_shader_data_type::float3, "a_position"},
        {e_shader_data_type::float4, "a_color"},
        {e_shader_data_type::float2, "a_tex_coord"},
        {e_shader_data_type::float1, "a_tex_index"},
        {e_shader_data_type::float1, "a_tiling_factor"},
    });
    s_data.quad_vertex_array->add_buffer(s_data.quad_vertex_buffer);

    s_data.quad_vertex_buffer_base = new quad_vertex[renderer_2d::max_vertices];

    uint32_t *quad_indices = new uint32_t[renderer_2d::max_indices];
    uint32_t offset = 0;
    for (uint32_t i = 0; i < renderer_2d::max_indices; i += s_quad_indices)
    {
        quad_indices[i + 0] = offset + 0;
        quad_indices[i + 1] = offset + 1;
        quad_indices[i + 2] = offset + 2;

        quad_indices[i + 3] = offset + 2;
        quad_indices[i + 4] = offset + 3;
        quad_indices[i + 5] = offset + 0;

        offset += s_quad_vertices;
    }


    const ref<index_buffer> quad_ib = 
        index_buffer::create(quad_indices, renderer_2d::max_indices);
    s_data.quad_vertex_array->add_buffer(quad_ib);
    delete[] quad_indices;

    texture_parameters params;
    params.wrap = e_texture_wrap::repeat;
    s_data.wite_texture = texture_2d::create(1, 1);
    uint32_t white_tex_data = 0xffffffff; // 2 Fs per channel
    s_data.wite_texture->data(&white_tex_data, sizeof(uint32_t));

    int32_t samplers[renderer_2d::max_texture_slots];
    for (uint32_t i = 0; i < renderer_2d::max_texture_slots; i++)
        samplers[i] = i;

    s_data.texture_shader = shader::create("assets/shaders/texture_2d.glsl");
    s_data.texture_shader->bind();
    s_data.texture_shader->set_int_array("u_textures", 
        samplers, renderer_2d::max_texture_slots);

    // always set texture slot 0 to full white
    s_data.texture_slots[0] = s_data.wite_texture;

    s_data.quad_vertex_positions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    s_data.quad_vertex_positions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
    s_data.quad_vertex_positions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
    s_data.quad_vertex_positions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
}

void pyro::renderer_2d::shutdown()
{
    PYRO_PROFILE_FUNCTION();
    delete[] s_data.quad_vertex_buffer_base;
}

void pyro::renderer_2d::begin_scene(camera &camera)
{
    PYRO_PROFILE_FUNCTION();
    s_data.texture_shader->bind();
    s_data.texture_shader->set_mat4("u_view_projection", 
        camera.view_projection_matrix());

    reset_render_data();
}

void pyro::renderer_2d::end_scene()
{
    PYRO_PROFILE_FUNCTION();
    
    // reinterpret_cast converts one pointer to another without changing 
    // the address, or converts between pointers and their numerical 
    // (integer) values
    auto next_available_slot = 
        reinterpret_cast<uint8_t*>(s_data.quad_vertex_buffer_ptr);
    auto full_array_size = 
        reinterpret_cast<uint8_t*>(s_data.quad_vertex_buffer_base);
    uint32_t data_size = 
        static_cast<uint32_t>(next_available_slot - full_array_size);
    s_data.quad_vertex_buffer->data(s_data.quad_vertex_buffer_base, data_size); 

    flush();
}

void pyro::renderer_2d::flush()
{
    if (s_data.quad_index_count == 0)
        return; // Nothing to draw

    // bind all textures slots
    for (uint32_t i = 0; i < s_data.texture_slot_index; i++)
        s_data.texture_slots[i]->bind(i);

    render_command::draw_indexed(
        s_data.quad_vertex_array, s_data.quad_index_count);
    s_data.stats.draw_calls++;
}

pyro::ref<pyro::shader> pyro::renderer_2d::current_shader()
{
    return s_data.texture_shader;
}

void pyro::renderer_2d::draw_quad(quad_properties const& props)
{
    PYRO_PROFILE_FUNCTION();

    if (s_data.quad_index_count >= max_indices)
    {
        end_scene();
        reset_render_data();
    }

    //TODO - textures are added even if already present in s_data.texture_slots!
    float tex_index = 0.0f; // if no new textures use default texture index
    texture_2d* tex_param = props.texture.get();
    for (uint32_t i = 1; i < s_data.texture_slot_index; i++)
    {
        // get the new texture
        texture_2d* inbuffer_tex = s_data.texture_slots[i].get();
        // if both texture existst
        if (inbuffer_tex && tex_param)
        {
            if (*inbuffer_tex == *tex_param)
            {
                tex_index = (float)i;
                break;
            }
        }
    }

    if(tex_index == 0.f)
    {
        if (tex_param)
        {
            tex_index = static_cast<float>(s_data.texture_slot_index);
            s_data.texture_slots[s_data.texture_slot_index] = props.texture;
            s_data.texture_slot_index++;
        }
    }

    constexpr glm::vec2 tex_coords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    glm::mat4 transform = glm::translate(glm::mat4(1.f), props.position);
    if(props.rotation != 0.f) // if no rotation avoid matrix calculation
        transform = glm::rotate(transform, props.rotation, pyro::math::axis_z);
    transform = glm::scale(transform, { props.size.x, props.size.y, 1.f });
    
    for (size_t i = 0; i < s_quad_vertices; i++)
    {
        s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertex_positions[i];
        s_data.quad_vertex_buffer_ptr->color = props.color;
        s_data.quad_vertex_buffer_ptr->tex_coord = tex_coords[i];
        s_data.quad_vertex_buffer_ptr->tex_index = tex_index;
        s_data.quad_vertex_buffer_ptr->tiling_factor = props.tiling_factor;
        s_data.quad_vertex_buffer_ptr++;
    }
    
    s_data.quad_index_count += s_quad_indices; // 6 indices per quad
    s_data.stats.quad_count++;
}

void pyro::renderer_2d::reset_stats()
{
    memset(&s_data.stats, 0, sizeof(statistics));
}

pyro::renderer_2d::statistics pyro::renderer_2d::stats()
{
    return s_data.stats;
}

void pyro::renderer_2d::reset_render_data()
{
    s_data.texture_slot_index = 1;
    s_data.quad_index_count = 0;
    s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;
}
