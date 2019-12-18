#include "layer_3d.h"
#include <imgui/imgui.h>

layer_3d::layer_3d()
    :m_3d_camera_controller(pyro::perspective_camera_controller::e_control_type::first_person,
                        static_cast<float>(pyro::application::window().width()),
                        static_cast<float>(pyro::application::window().height()))
{
    m_color_shader = m_shader_library.load("assets/shaders/color.glsl");
    m_textured_shader = m_shader_library.load("assets/shaders/texture.glsl");

    //======= triangle ========= 
    float vertices[3 * 7]
    {
        -.5f, -.5f, .0f,    .8f, .2f, .8f, 1.0f,
         .5f, -.5f, .0f,    .2f, .3f, .8f, 1.0f,
         .0f,  .5f, .0f,    .8f, .8f, .2f, 1.0f,
    };

    pyro::ref<pyro::vertex_buffer> triangle_vb(pyro::vertex_buffer::create(vertices, sizeof(vertices)));

    const pyro::buffer_layout layout
    {
        {pyro::e_shader_data_type::float3, "a_position"},
        {pyro::e_shader_data_type::float2, "a_tex_coord"},
    };
    triangle_vb->layout(layout);

    uint32_t indices[3]{0,1,2};
    const pyro::ref<pyro::index_buffer> triangle_ib(pyro::index_buffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

    m_triangle_va = pyro::vertex_array::create();
    m_triangle_va->add_buffer(triangle_vb);
    m_triangle_va->add_buffer(triangle_ib);

    //======== rectangle ========= 

    float rect_vertices[]
    {
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
    };

    const pyro::ref<pyro::vertex_buffer> rect_vb = pyro::vertex_buffer::create(rect_vertices, sizeof(rect_vertices));

    uint32_t rect_indices[]{0,1,2, 2,3,0};
    const pyro::ref<pyro::index_buffer> rect_ib = pyro::index_buffer::create(rect_indices, sizeof(rect_indices) / sizeof(uint32_t));

    rect_vb->layout({
        {pyro::e_shader_data_type::float3, "a_position"},
        {pyro::e_shader_data_type::float2, "a_tex_coord"},
    });

    m_rect_va = pyro::vertex_array::create();
    m_rect_va->add_buffer(rect_vb);
    m_rect_va->add_buffer(rect_ib);

    //======== cube ========= 

    float cube_vertices[]
    {
        // FRONT 
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // A 
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // B 
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // C 
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // D  

        // BACK 
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // E 
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // F 
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // G 
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // H 

        // LEFT  
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // E = 8 
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // H = 9 
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // D = 10 
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // A = 11 

        // RIGHT 
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // F = 12 
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // G = 13 
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // C = 14 
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // B = 15 

        // BOTTOM 
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // E = 16 
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // F = 17 
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // B = 18 
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // A = 19 

        // TOP 
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, // G = 20 
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, // H = 21 
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // D = 22 
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // C = 23 
    };

    const pyro::ref<pyro::vertex_buffer> cube_vb = pyro::vertex_buffer::create(cube_vertices, sizeof(cube_vertices));

    uint32_t cube_indices[]
    {
        0, 1, 2,    2, 3, 0, // FRONT 
        5, 4, 7,    7, 6, 5, // BACK 
        8,11,10,   10, 9, 8, // LEFT 
        15,12,13,   13,14,15, // RIGHT 
        16,17,18,   18,19,16, // BOTTOM 
        21,20,23,   23,22,21  // TOP 

    };
    const pyro::ref<pyro::index_buffer> cube_ib = pyro::index_buffer::create(cube_indices, sizeof(cube_indices) / sizeof(uint32_t));

    cube_vb->layout({
        {pyro::e_shader_data_type::float3, "a_position"},
        {pyro::e_shader_data_type::float2, "a_tex_coord"},
    });

    m_cube_va = pyro::vertex_array::create();
    m_cube_va->add_buffer(cube_vb);
    m_cube_va->add_buffer(cube_ib);

    m_textured_shader->bind();
    m_textured_shader->set_int("u_sampler", 0);

    m_texture = pyro::texture_2d::create_from_file("assets/textures/checkerboard.png");
    m_face_texture = pyro::texture_2d::create_from_file("assets/textures/face.png");
}

void layer_3d::on_update(const pyro::timestep &timestep)
{
    m_3d_camera_controller.on_update(timestep);
    //m_3d_camera.on_update(timestep);
}

void layer_3d::on_imgui_render()
{

    pyro::render_command::clear_color({0.2f, 0.3f, 0.3f, 1.0f});
    pyro::render_command::clear();

    pyro::renderer::begin_scene(m_3d_camera_controller.camera(), m_textured_shader);

    glm::mat4 transform(1.0f);
    // big square 
    m_texture->bind();
    pyro::renderer::submit(m_textured_shader, m_rect_va, transform);
    m_face_texture->bind();
    pyro::renderer::submit(m_textured_shader, m_rect_va, transform);

    pyro::renderer::end_scene();

}

void layer_3d::on_event(pyro::event &event)
{
    if(event.event_type() == pyro::event_type_e::key_pressed)
    {
        auto &e = dynamic_cast<pyro::key_pressed_event &>(event);
        if(e.key_code() == pyro::key_codes::KEY_TAB)
        {
            pyro::render_command::toggle_wireframe();
        }
        //PYRO_TRACE("{0}", static_cast<char>(e.key_code())); 
    }
    m_3d_camera_controller.on_event(event);

}
