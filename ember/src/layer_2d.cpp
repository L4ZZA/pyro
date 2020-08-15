#include "pyro.h"
#include "layer_2d.h"
#include "imgui/imgui.h"
#include "scenes/noise1d_scene.h" 
#include "scenes/noise2d_scene.h" 
#include "scenes/roguelike_scene.h" 
#include "utils/random.h" 


layer_2d::layer_2d(float width, float height)
    : imgui_layer("Sandbox2D")
    , m_seed(0)
{
    m_2d_camera_controller =
        pyro::make_ref<pyro::orthographic_camera_controller>(
            glm::vec3{ 0.f,0.f,0.f }, width / height, 10.f);
    m_scene_manager.add_scene(pyro::make_ref<noise1d_scene>(m_2d_camera_controller));
    m_scene_manager.add_scene(pyro::make_ref<noise2d_scene>(m_2d_camera_controller));
    m_scene_manager.add_scene(pyro::make_ref<roguelike_scene>(m_2d_camera_controller));
    pyro::framebuffer_props props;
    props.width =  static_cast<uint32_t>(width);
    props.height = static_cast<uint32_t>(height);
    m_framebuffer = pyro::frame_buffer_2d::create(props);
}

layer_2d::~layer_2d()
{
}


void layer_2d::on_attach()
{
    PYRO_PROFILE_FUNCTION();
    imgui_layer::on_attach();
    m_scene_manager.init_first_scene();
}

void layer_2d::on_detach()
{
    PYRO_PROFILE_FUNCTION();
}

void layer_2d::on_update(const pyro::timestep &ts)
{
    // Update
    PYRO_PROFILE_FUNCTION();
    m_2d_camera_controller->on_update(ts);
    m_scene_manager.on_update(ts);
}

void layer_2d::on_render() const
{
    pyro::renderer_2d::reset_stats();
    {
        // Pre Render
        PYRO_PROFILE_SCOPE("scene::pre_render");
        m_framebuffer->bind();
        pyro::render_command::clear_color({ 0.1f, 0.1f, 0.1f, 1.f });
        pyro::render_command::clear();
    }
    {
        // Render
        PYRO_PROFILE_SCOPE("layer_2d::render");
        m_scene_manager.on_render();
        m_framebuffer->unbind();
    }
}

void layer_2d::on_imgui_render()
{
    // Note: Switch this to true to enable dockspace
    static bool dockingEnabled = true;
    if(dockingEnabled)
    {
        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if(opt_fullscreen)
        {
            ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        {
            ImGui::PopStyleVar();

            if(opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO &io = ImGui::GetIO();
            if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            if(ImGui::BeginMenuBar())
            {
                if(ImGui::BeginMenu("File"))
                {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows, 
                    // which we can't undo at the moment without finer window depth/z control.
                    //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                    if(ImGui::MenuItem("Exit")) pyro::application::instance().exit();
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            ImGui::Begin("Settings");

            auto stats = pyro::renderer_2d::stats();
            ImGui::Text("-- 2D Renderer stats:");
            float ms = pyro::application::frame_time() * 1000.f;
            ImGui::Text("- Frame time: %f ms", ms);
            ImGui::Text("- FPS: %d/s", pyro::application::fps());
            ImGui::Text("- Draw calls: %d", stats.draw_calls);
            ImGui::Text("- Quads: %d", stats.quad_count);
            ImGui::Text("- Vertices: %d", stats.total_vertex_count());
            ImGui::Text("- Indices: %d", stats.total_index_count());

            uint32_t textureID = m_framebuffer->color_attachment();
            ImGui::Image((void *)textureID, ImVec2{ (float)m_framebuffer->width(), (float)m_framebuffer->height() });
            ImGui::End();
        }
        ImGui::End();

    }
    else
    {

        auto current_scene = std::static_pointer_cast<base_noise_scene>(m_scene_manager.current_scene());

        // hide all ui if the scene is being played
        if(current_scene->is_playing())
            return;

        ImGui::Begin("Settings");

        auto stats = pyro::renderer_2d::stats();
        ImGui::Text("-- 2D Renderer stats:");
        float ms = pyro::application::frame_time() * 1000.f;
        ImGui::Text("- Frame time: %f ms", ms);
        ImGui::Text("- FPS: %d/s", pyro::application::fps());
        ImGui::Text("- Draw calls: %d", stats.draw_calls);
        ImGui::Text("- Quads: %d", stats.quad_count);
        ImGui::Text("- Vertices: %d", stats.total_vertex_count());
        ImGui::Text("- Indices: %d", stats.total_index_count());
        ImGui::Text("---------------------");

        ImGui::Text("Select level type");
        static int scene_index = 0;
        bool pressed = false;
        pressed |= ImGui::RadioButton("1", &scene_index, 0); ImGui::SameLine();
        pressed |= ImGui::RadioButton("2", &scene_index, 1); ImGui::SameLine();
        pressed |= ImGui::RadioButton("3", &scene_index, 2);

        if(pressed)
        {
            m_scene_manager.go_to(scene_index);
        }

        //ImGui::Text("[Press Q to quit play mode]");
        //if(ImGui::Button("PLAY", { 100.f,25.f })) 
        //{
        //    current_scene->play();
        //}
        ImGui::Text("---------------------");

        m_scene_manager.on_imgui_render();

        ImGui::End();
    }
}

void layer_2d::on_event(pyro::event &e)
{
    imgui_layer::on_event(e);
    m_2d_camera_controller->on_event(e);
    pyro::event_dispatcher dispatcher(e);
    // dispatch event on window X pressed 
    auto current_scene = std::static_pointer_cast<base_noise_scene>(m_scene_manager.current_scene());
    dispatcher.dispatch<pyro::key_pressed_event>([&](pyro::key_pressed_event ev)
        {
            if(current_scene->is_playing() && ev.key_code() == pyro::key_codes::KEY_Q)
            {
                current_scene->stop_playing();
            }
            // return if event is handled or not
            return false;
        });

    m_scene_manager.on_event(e);
}

