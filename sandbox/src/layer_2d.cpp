#include "layer_2d.h"
#include "camera_controller.h"
#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>

layer_2d::layer_2d(uint32_t width, uint32_t height) 
    : layer(width, height, "sandbox_2d_layer")
{
}

layer_2d::~layer_2d()
{
}

void layer_2d::on_attach()
{
    //imgui_layer::on_attach();
    
    //pyro::framebuffer_props props;
    //props.width = static_cast<uint32_t>(m_layer_width);
    //props.height = static_cast<uint32_t>(m_layer_height);
    //m_framebuffer = pyro::frame_buffer_2d::create(props);
    
    pyro::texture_parameters params;
    params.wrap = pyro::e_texture_wrap::repeat;
    m_checkerboard_texture = pyro::texture_2d::create_from_file("assets/textures/checkerboard.png", params);
    m_sprite_sheet = pyro::texture_2d::create_from_file("assets/textures/RPGpack_sheet_2X.png", params);

    m_barrel_texture = pyro::sub_texture_2d::create_from_coords(m_sprite_sheet, { 8,2 }, {128.f, 128.f});
    m_tree_texture = pyro::sub_texture_2d::create_from_coords(m_sprite_sheet, { 2,1 }, { 128.f, 128.f }, { 1.f,2.f });
    
    m_active_scene = pyro::make_ref<pyro::scene>();
    
    auto green_square = m_active_scene->create_entity("Green Square", {1.f,0.f,0.f});
    green_square.add_component<pyro::sprite_renderer_component>(m_barrel_texture);
    auto red_square = m_active_scene->create_entity("Red Square", {0.f,.5f,0.f});
    red_square.add_component<pyro::sprite_renderer_component>(m_tree_texture);
    auto &transform = red_square.get_component<pyro::transform_component>().transform;
    transform = glm::scale(transform, { 1.f,2.f,0.f });
    
    m_camera_entity = m_active_scene->create_entity("Camera Entity");
    m_camera_entity.add_component<pyro::camera_component>();

    m_camera_entity.add_component<pyro::native_script_component>().bind<camera_controller_ortho>();
}

void layer_2d::on_detach()
{
    PYRO_PROFILE_FUNCTION();
}

void layer_2d::on_update(const pyro::timestep &ts)
{

    if(m_layer_width > 0 && m_layer_height > 0 &&
        (m_active_scene->width() != m_layer_width
            || m_active_scene->height() != m_layer_height))
    {
        m_active_scene->on_viewport_resize(m_layer_width, m_layer_height);
    }
    if(m_viewport_focused)
    {
        m_active_scene->on_update(ts);
    }

    //
    //props.position = {1.f, 0.0f, 0.f};
    //props.size = {0.8f, 0.85f};
    //props.rotation = -45.f;
    //props.color = { 0.8f, 0.2f, 0.3f, 1.0f };
    //auto &cmp = m_square_entity.get_component<pyro::transform_component>();
    //cmp.transform = 

    //props.position = {-1.0f, 0.0f, 0.f};
    //props.size = {0.8f, 0.8f};
    //props.rotation = 0.f;
    //props.color = {0.8f, 0.3f, 0.2f, 1.0f};
    //pyro::renderer_2d::draw_quad(props);

    //props.position = {0.5f, -0.5f, 0.f};
    //props.size = {0.5f, 0.75f};
    //props.color = {m_rect_color, 1.f};
    //pyro::renderer_2d::draw_quad(props);

    //props.position = { 0.f, 0.f, -0.1f };
    //props.size = {20.f, 20.f};
    //props.color = glm::vec4(1.f);
    //props.tiling_factor = 10.f;
    //props.texture = m_checkerboard_texture;
    //pyro::renderer_2d::draw_quad(props);

    //props.color = glm::vec4(1);
    //props.position = { -2.f, 0.f, 0.0f };
    //props.size = {1.f, 1.f};
    //props.rotation = m_rect_rotation;
    //props.tiling_factor = 20.f;
    //props.color = { .8f, 1.f, .8f, 1.f };
    //pyro::renderer_2d::draw_quad(props);

    //for (float y = -5.0f; y < 5.0f; y += 0.5f)
    //{
    //    for (float x = -5.0f; x < 5.0f; x += 0.5f)
    //    {
    //        pyro::quad_properties props;
    //        props.color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
    //        props.size = { 0.45f, 0.45f };
    //        props.position = { x, y, 0.f };
    //        pyro::renderer_2d::draw_quad(props);
    //    }
    //}
}

void layer_2d::on_render() const
{
    pyro::renderer_2d::reset_stats();
    {
        // Pre Render
        //m_framebuffer->bind();
        pyro::render_command::clear_color({0.1f, 0.1f, 0.1f, 1});
        pyro::render_command::clear();
    }
    {
        // Render
        m_active_scene->on_render();
        
        //m_framebuffer->unbind();
    }
}

void layer_2d::on_event(pyro::event &e)
{
    m_active_scene->on_event(e);
}
