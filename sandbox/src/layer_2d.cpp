#include "layer_2d.h"
#include "imgui/imgui.h"

layer_2d::layer_2d() : imgui_layer("Sandbox2D"),
m_2d_camera_controller(1280.0f / 720.0f, true)
{
}

layer_2d::~layer_2d()
{
}

void layer_2d::on_attach()
{
    imgui_layer::on_attach();
    // ======= shaders ===========
    //m_flat_color_shader = pyro::shader::create("uniform_color", flat_color_vertex_shader, flat_color_fragment_shader);
    m_flat_color_shader = pyro::shader_library::load("assets/shaders/dynamic_flat_color.glsl");

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
    m_checkerboard_texture = pyro::texture_2d::create("assets/textures/Checkerboard.png");
}

void layer_2d::on_detach()
{
}

void layer_2d::on_update(const pyro::timestep &ts)
{
    // Update
    m_2d_camera_controller.on_update(ts);

    // Render
    pyro::render_command::clear_color({0.8f, 0.2f, 0.3f, 1});
    pyro::render_command::clear();

    //pyro::renderer_2d::BeginScene(m_CameraController.camera());
    //pyro::renderer_2d::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
    //pyro::renderer_2d::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});
    //pyro::renderer_2d::DrawQuad({0.0f, 0.0f, -0.1f}, {10.0f, 10.0f}, m_CheckerboardTexture);
    //pyro::renderer_2d::EndScene();
    
    // old way
    pyro::renderer::begin_scene(m_2d_camera_controller.camera(), m_flat_color_shader);

    static auto scale = glm::scale(glm::mat4(1), glm::vec3(0.1f));

    m_flat_color_shader->set_vec3("u_color", m_rect_color);

    for(int y = 0; y < 20; y++)
    {
        for(int x = 0; x < 20; x++)
        {
            glm::vec3 pos(x * 0.11f, y * 0.11f, 0);
            auto transform = glm::translate(glm::mat4(1), m_rect_pos + pos) * scale;
            pyro::renderer::submit(m_flat_color_shader, m_rect_va, transform);
        }
    }

    pyro::renderer::end_scene();
}

void layer_2d::on_imgui_render()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Squares color", glm::value_ptr(m_rect_color));
    ImGui::End();
}

void layer_2d::on_event(pyro::event &e)
{
    m_2d_camera_controller.on_event(e);
}
