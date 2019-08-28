#include "main_layer.h"
#include <imgui/imgui.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

//glm::mat4 camera(float Translate, glm::vec2 const & Rotate)
//{
//    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
//    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
//    View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
//    View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
//    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
//    return Projection * View * Model;
//}

static const std::string vertex_shader = R"(
    #version 430

    layout(location = 0) in vec3 a_position;
    layout(location = 1) in vec4 a_color;

    uniform mat4 u_view_projection;
    uniform mat4 u_transform;

    out vec3 v_position;
    out vec4 v_color;

    void main()
    {
        v_position = a_position;
        v_color = a_color;
        gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
    }
)";

static const std::string fragment_shader = R"(
    #version 430

    layout(location = 0) out vec4 o_color;

    in vec3 v_position;
    in vec4 v_color;

    void main()
    {
        o_color = v_color;
    }
)";

static const std::string flat_color_vertex_shader = R"(
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
)";

static const std::string flat_color_fragment_shader = R"(
    #version 430

    layout(location = 0) out vec4 o_color;

    in vec3 v_position;
    
    uniform vec3 u_color;

    void main()
    {
        o_color = vec4(u_color, 1.f);
    }
)";

static const std::string textured_vertex_shader_3d = R"( 
    #version 430 
 
    layout(location = 0) in vec3 a_position; 
    layout(location = 1) in vec2 a_tex_coord; 
 
    uniform mat4 u_view_projection; 
    uniform mat4 u_transform; 
 
    out vec2 v_tex_coord;  
 
    void main() 
    { 
        v_tex_coord = a_tex_coord; 
        gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0); 
    } 
)";

static const std::string textured_fragment_shader_3d = R"( 
    #version 430 
 
    layout(location = 0) out vec4 o_color; 
 
    in vec2 v_tex_coord; 
 
    uniform sampler2D u_sampler;  
 
    void main() 
    { 
        o_color = texture(u_sampler, v_tex_coord);
    } 
)";

example_layer::example_layer()
    :m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f),
    m_3d_camera(pyro::application::instance().get_window().width(), pyro::application::instance().get_window().height())
{
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
        {pyro::e_shader_data_type::float4, "a_color"},
    };
    triangle_vb->layout(layout);

    uint32_t indices[3]{0,1,2};
    const pyro::ref<pyro::index_buffer> triangle_ib(pyro::index_buffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

    m_triangle_va.reset(pyro::vertex_array::create());
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

    const pyro::ref<pyro::vertex_buffer> rect_vb(pyro::vertex_buffer::create(rect_vertices, sizeof(rect_vertices)));

    uint32_t rect_indices[]{0,1,2, 2,3,0};
    const pyro::ref<pyro::index_buffer> rect_ib(pyro::index_buffer::create(rect_indices, sizeof(rect_indices) / sizeof(uint32_t)));

    rect_vb->layout({
        {pyro::e_shader_data_type::float3, "a_position"},
        {pyro::e_shader_data_type::float2, "a_tex_coord"},
        });

    m_rect_va.reset(pyro::vertex_array::create());
    m_rect_va->add_buffer(rect_vb);
    m_rect_va->add_buffer(rect_ib);

    //======== cube =========

    float cube_vertices[]
    {
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
    };

    const pyro::ref<pyro::vertex_buffer> cube_vb(pyro::vertex_buffer::create(cube_vertices, sizeof(cube_vertices)));

    uint32_t cube_indices[]{0,1,2, 2,3,0};
    const pyro::ref<pyro::index_buffer> cube_ib(pyro::index_buffer::create(cube_indices, sizeof(cube_indices) / sizeof(uint32_t)));

    cube_vb->layout({
        {pyro::e_shader_data_type::float3, "a_position"},
        {pyro::e_shader_data_type::float2, "a_tex_coord"},
        });

    m_cube_va.reset(pyro::vertex_array::create());
    m_cube_va->add_buffer(cube_vb);
    m_cube_va->add_buffer(cube_ib);

    m_color_shader.reset(new pyro::gl_shader(vertex_shader, fragment_shader));
    m_flat_color_shader.reset(new pyro::gl_shader(flat_color_vertex_shader, flat_color_fragment_shader));
    m_textured_shader.reset(new pyro::gl_shader(textured_vertex_shader_3d, textured_fragment_shader_3d));

    std::dynamic_pointer_cast<pyro::gl_shader>(m_textured_shader)->bind();
    std::dynamic_pointer_cast<pyro::gl_shader>(m_textured_shader)->set_uniform("u_sampler", 0);

    m_texture = pyro::texture_2d::create("assets/textures/checkerboard.png");
    m_face_texture = pyro::texture_2d::create("assets/textures/face.png");
}

void example_layer::on_attach()
{
    imgui_layer::on_attach();
}

void example_layer::on_detach()
{
    imgui_layer::on_detach();
}

void example_layer::on_update(pyro::timestep timestep)
{
    if(pyro::input::key_pressed(pyro::key_codes::KEY_A)) // left
        m_3d_camera.move(pyro::perspective_camera::e_direction::left, timestep);
    else if(pyro::input::key_pressed(pyro::key_codes::KEY_D)) // right
        m_3d_camera.move(pyro::perspective_camera::e_direction::right, timestep);

    if(pyro::input::key_pressed(pyro::key_codes::KEY_S)) // down
        m_3d_camera.move(pyro::perspective_camera::e_direction::back, timestep);
    else if(pyro::input::key_pressed(pyro::key_codes::KEY_W)) // up
        m_3d_camera.move(pyro::perspective_camera::e_direction::front, timestep);

    if(pyro::input::key_pressed(pyro::key_codes::KEY_Q)) // anticlockwise rotation
        m_3d_camera.rotate(pyro::camera::e_rotation::anticlock_wise, pyro::camera::e_axis::z, timestep);
    else if(pyro::input::key_pressed(pyro::key_codes::KEY_E)) // clockwise rotation
        m_3d_camera.rotate(pyro::camera::e_rotation::clock_wise, pyro::camera::e_axis::z, timestep);

    if(pyro::input::key_pressed(pyro::key_codes::KEY_LEFT)) // left
        m_rect_pos.x -= m_rect_speed * timestep;
    else if(pyro::input::key_pressed(pyro::key_codes::KEY_RIGHT)) // right
        m_rect_pos.x += m_rect_speed * timestep;

    if(pyro::input::key_pressed(pyro::key_codes::KEY_DOWN)) // down
        m_rect_pos.y -= m_rect_speed * timestep;
    else if(pyro::input::key_pressed(pyro::key_codes::KEY_UP)) // up
        m_rect_pos.y += m_rect_speed * timestep;
}

void example_layer::on_imgui_render()
{
    static bool show = true;
    ImGui::ShowDemoWindow(&show);

    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Squares color", glm::value_ptr(m_rect_color));
    ImGui::End();


    pyro::render_command::clear_color({0.1f, 0.1f, 0.1f, 1.f});
    pyro::render_command::clear();

    pyro::renderer::begin_scene(m_3d_camera, m_textured_shader);

    // big square
    m_texture->bind();
    pyro::renderer::submit(m_textured_shader, m_cube_va, glm::scale(glm::mat4(1), glm::vec3(1.5f)));
    m_face_texture->bind();
    pyro::renderer::submit(m_textured_shader, m_cube_va, glm::scale(glm::mat4(1), glm::vec3(1.5f)));

    pyro::renderer::end_scene();



    pyro::renderer::begin_scene(m_2d_camera, m_flat_color_shader);

    static auto scale = glm::scale(glm::mat4(1), glm::vec3(0.1f));

    std::dynamic_pointer_cast<pyro::gl_shader>(m_flat_color_shader)->set_uniform("u_color", m_rect_color);

    for(int y = 0; y < 20; y++)
        for(int x = 0; x < 20; x++)
        {
            glm::vec3 pos(x * 0.11f, y * 0.11f, 0);
            auto transform = glm::translate(glm::mat4(1), m_rect_pos + pos) * scale;
            pyro::renderer::submit(m_flat_color_shader, m_rect_va, transform);
        }

    // triangle
    //pyro::renderer::submit(m_triangle_shader, m_vertex_array);

    pyro::renderer::end_scene();
}

void example_layer::on_event(pyro::event& event)
{
    if (event.event_type() == pyro::event_type_e::key_pressed)
    {
        auto& e = dynamic_cast<pyro::key_pressed_event&>(event); 
        if (e.key_code() == pyro::key_codes::KEY_TAB)
        {
            pyro::render_command::toggle_wireframe();
        }
        //PYRO_TRACE("{0}", static_cast<char>(e.key_code()));
    }

}
