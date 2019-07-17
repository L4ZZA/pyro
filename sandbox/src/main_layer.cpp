#include "main_layer.h"
#include "imgui.h"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

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

    out vec3 v_position;
    out vec4 v_color;

    void main()
    {
        v_position = a_position;
        v_color = a_color;
        gl_Position = u_view_projection * vec4(a_position, 1.0);
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

static const std::string rect_vertex_shader = R"(
    #version 430

    layout(location = 0) in vec3 a_position;

    uniform mat4 u_view_projection;

    out vec3 v_position;

    void main()
    {
        v_position = a_position;
        gl_Position = u_view_projection * vec4(a_position, 1.0);
    }
)";

static const std::string rect_fragment_shader = R"(
    #version 430

    layout(location = 0) out vec4 o_color;

    in vec3 v_position;

    void main()
    {
        o_color = vec4(.9f, .1f, .6f, 1.f);
    }
)";

example_layer::example_layer()
    :m_camera(-1.6f, 1.6f, -0.9f, 0.9f)
{
    float vertices[3 * 7]
    {
        -.5f, -.5f, .0f,    .8f, .2f, .8f, 1.0f,
         .5f, -.5f, .0f,    .2f, .3f, .8f, 1.0f,
         .0f,  .5f, .0f,    .8f, .8f, .2f, 1.0f,
    };

    std::shared_ptr<pyro::vertex_buffer> vb(pyro::vertex_buffer::create(vertices, sizeof(vertices)));

    const pyro::buffer_layout layout
    {
        {pyro::e_shader_data_type::float3, "a_position"},
        {pyro::e_shader_data_type::float4, "a_color"},
    };
    vb->layout(layout);

    uint32_t indices[3]{ 0,1,2 };
    const std::shared_ptr<pyro::index_buffer> ib(pyro::index_buffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

    m_vertex_array.reset(pyro::vertex_array::create());
    m_vertex_array->add_buffer(vb);
    m_vertex_array->add_buffer(ib);


    float rect_vertices[]
    {
        -0.75f, -0.75f, 0.0f,
         0.75f, -0.75f, 0.0f,
         0.75f,  0.75f, 0.0f,
        -0.75f,  0.75f, 0.0f,
    };

    const std::shared_ptr<pyro::vertex_buffer> rect_vb(pyro::vertex_buffer::create(rect_vertices, sizeof(rect_vertices)));

    uint32_t rect_indices[]{ 0,1,2, 2,3,0 };
    const std::shared_ptr<pyro::index_buffer> rect_ib(pyro::index_buffer::create(rect_indices, sizeof(rect_indices) / sizeof(uint32_t)));

    rect_vb->layout({
        {pyro::e_shader_data_type::float3, "a_position"},
    });

    m_rect_va.reset(pyro::vertex_array::create());
    m_rect_va->add_buffer(rect_vb);
    m_rect_va->add_buffer(rect_ib);

    m_shader.reset(new pyro::gl_shader(vertex_shader, fragment_shader));
    m_blue_shader.reset(new pyro::gl_shader(rect_vertex_shader, rect_fragment_shader));   
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
    if (pyro::input::key_pressed(pyro::key_codes::KEY_A)) // left
        m_camera.move(pyro::camera::e_direction::left, timestep); 
    else if (pyro::input::key_pressed(pyro::key_codes::KEY_D)) // right
        m_camera.move(pyro::camera::e_direction::right, timestep); 

    if (pyro::input::key_pressed(pyro::key_codes::KEY_S)) // bottom
        m_camera.move(pyro::camera::e_direction::down, timestep); 
    else if (pyro::input::key_pressed(pyro::key_codes::KEY_W)) // top
        m_camera.move(pyro::camera::e_direction::up, timestep); 

    if (pyro::input::key_pressed(pyro::key_codes::KEY_Q)) // anticlockwise rotation
        m_camera.rotate(pyro::camera::e_rotation::anticlock_wise, timestep); 
    else if (pyro::input::key_pressed(pyro::key_codes::KEY_E)) // clockwise rotation
        m_camera.rotate(pyro::camera::e_rotation::clock_wise, timestep); 
}

void example_layer::on_imgui_render()
{
    static bool show = true;
    ImGui::ShowDemoWindow(&show);

    ImGui::Begin("Test");
    ImGui::Text("Rendered text with ImGui.");
    ImGui::End();


    pyro::render_command::clear_color({ 0.1f, 0.1f, 0.1f, 1.f });
    pyro::render_command::clear();

    pyro::renderer::begin_scene(m_camera);

    pyro::renderer::submit(m_blue_shader, m_rect_va);
    pyro::renderer::submit(m_shader, m_vertex_array);

    pyro::renderer::end_scene();
}

void example_layer::on_event(pyro::event& p_event)
{
}
