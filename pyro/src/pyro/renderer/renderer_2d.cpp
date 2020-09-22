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

    struct renderer_2d_data
    {
        ref<vertex_array> quad_vertex_array;
        ref<vertex_buffer> quad_vertex_buffer;
        ref<shader> texture_shader;
        ref<texture_2d> wite_texture;

        uint32_t quad_index_count = 0;
        quad_vertex *quad_vertex_buffer_base = nullptr;
        quad_vertex *quad_vertex_buffer_ptr  = nullptr;

		std::array<ref<texture_2d>, renderer_2d::s_max_texture_slots> texture_slots;
		// Index of the next empty texture slot
        // 0 white texture index [default]
        uint32_t texture_slot_index = 1;

        glm::vec4 quad_vertex_positions[4];

        renderer_2d::statistics stats;
    };

    static renderer_2d_data s_data;
}

void pyro::renderer_2d::init()
{
    PYRO_PROFILE_FUNCTION();

    s_data.quad_vertex_array = vertex_array::create();

    s_data.quad_vertex_buffer =
        vertex_buffer::create(renderer_2d::s_max_vertices * sizeof(quad_vertex));
    s_data.quad_vertex_buffer->layout({
        {e_shader_data_type::float3, "a_position"},
        {e_shader_data_type::float4, "a_color"},
        {e_shader_data_type::float2, "a_tex_coord"},
        {e_shader_data_type::float1, "a_tex_index"},
        {e_shader_data_type::float1, "a_tiling_factor"},
        });
    s_data.quad_vertex_array->add_buffer(s_data.quad_vertex_buffer);

    s_data.quad_vertex_buffer_base = new quad_vertex[renderer_2d::s_max_vertices];

    uint32_t *quad_indices = new uint32_t[renderer_2d::s_max_indices];
    uint32_t offset = 0;
    for(uint32_t i = 0; i < renderer_2d::s_max_indices; i += s_quad_indices)
    {
        quad_indices[i + 0] = offset + 0;
        quad_indices[i + 1] = offset + 1;
        quad_indices[i + 2] = offset + 2;

        quad_indices[i + 3] = offset + 2;
        quad_indices[i + 4] = offset + 3;
        quad_indices[i + 5] = offset + 0;

        offset += s_quad_vertex_count;
    }


    const ref<index_buffer> quad_ib =
        index_buffer::create(quad_indices, renderer_2d::s_max_indices);
    s_data.quad_vertex_array->add_buffer(quad_ib);
    delete[] quad_indices;
    s_data.wite_texture = texture_2d::create(1, 1);
    uint32_t white_tex_data = 0xffffffff; // 2 Fs per channel
    s_data.wite_texture->data(&white_tex_data, sizeof(uint32_t));

    int32_t samplers[renderer_2d::s_max_texture_slots];
    for(uint32_t i = 0; i < renderer_2d::s_max_texture_slots; i++)
        samplers[i] = i;

    s_data.texture_shader = shader::create("assets/shaders/texture_2d.glsl");
    s_data.texture_shader->bind();
    s_data.texture_shader->set_int_array("u_textures",
        samplers, renderer_2d::s_max_texture_slots);

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

void pyro::renderer_2d::begin_scene(ref<camera> camera)
{
    PYRO_PROFILE_FUNCTION();
    s_data.texture_shader->bind();
    s_data.texture_shader->set_mat4("u_view_projection", 
        camera->view_projection_matrix());
    s_data.texture_shader->set_int("u_grayscale", false);

    reset_render_data();
}

void pyro::renderer_2d::begin_scene(editor_camera const &camera, glm::mat4 const &transform)
{
    glm::mat4 viewProj = camera.projection_matrix() *glm::inverse(transform);
    s_data.texture_shader->bind();
    s_data.texture_shader->set_mat4("u_view_projection", viewProj);
    s_data.texture_shader->set_int("u_grayscale", false);

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
    if(s_data.quad_index_count == 0)
        return; // Nothing to draw

    // bind all textures slots
    for(uint32_t i = 0; i < s_data.texture_slot_index; i++)
        s_data.texture_slots[i]->bind(i);

    render_command::draw_indexed(
        s_data.quad_vertex_array, s_data.quad_index_count);
    s_data.stats.draw_calls++;
}

pyro::ref<pyro::shader> const &pyro::renderer_2d::current_shader()
{
    return s_data.texture_shader;
}

void pyro::renderer_2d::draw_quad(
    glm::mat4 const &transform,
    glm::vec4 color /*= { 1.0f, 1.0f, 1.0f, 1.0f }*/,
    ref<texture_2d> texture /*= nullptr*/,
    float textureIndex /*= 0.f*/,
    float tiling_factor /*= 1.f*/)
{
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    for(size_t i = 0; i < s_quad_vertex_count; i++)
    {
        s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertex_positions[i];
        s_data.quad_vertex_buffer_ptr->color = color;
        s_data.quad_vertex_buffer_ptr->tex_coord = textureCoords[i];
        s_data.quad_vertex_buffer_ptr->tex_index = textureIndex;
        s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
        s_data.quad_vertex_buffer_ptr++;
    }

    s_data.quad_index_count += 6;

    s_data.stats.quad_count++;
}

void pyro::renderer_2d::draw_quad(quad_properties const &props)
{
    PYRO_PROFILE_FUNCTION();

    if(s_data.quad_index_count >= renderer_2d::s_max_indices)
        flush_and_reset();

    float textureIndex = 0.0f;
    if(props.texture)
    {
        for(uint32_t i = 1; i < s_data.texture_slot_index; i++)
        {
            int32_t cached_id = s_data.texture_slots[i]->id();
            int32_t new_id = props.texture->id();
            if(cached_id == new_id)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if(textureIndex == 0.0f)
        {
            if(s_data.texture_slot_index >= renderer_2d::s_max_texture_slots)
                flush_and_reset();

            textureIndex = (float)s_data.texture_slot_index;
            s_data.texture_slots[s_data.texture_slot_index] = props.texture;
            s_data.texture_slot_index++;
        }
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), props.position);
    if(props.rotation != 0.f) // if no rotation avoid matrix calculation 
        transform = glm::rotate(transform, glm::radians(props.rotation), { 0.0f, 0.0f, 1.0f });
    transform = glm::scale(transform, { props.size.x, props.size.y, 1.0f });

    draw_quad(transform, props.color, props.texture, textureIndex, props.tiling_factor);
}

void pyro::renderer_2d::reset_render_data()
{
    s_data.quad_index_count = 0;
    s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;
    s_data.texture_slot_index = 1;
}

void pyro::renderer_2d::flush_and_reset()
{
    end_scene();
    reset_render_data();
}

void pyro::renderer_2d::reset_stats()
{
    memset(&s_data.stats, 0, sizeof(statistics));
}

pyro::renderer_2d::statistics pyro::renderer_2d::stats()
{
    return s_data.stats;
}

