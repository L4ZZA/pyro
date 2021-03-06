#include "pyro.h"
#include "editor_layer.h"
#include "imgui/imgui.h"
#include "scenes/noise1d_scene.h" 
#include "scenes/noise2d_scene.h" 
#include "scenes/roguelike_scene.h" 
#include "utils/random.h" 

#include <glm/gtc/type_ptr.hpp>


namespace pyro
{
    editor_layer::editor_layer(float width, float height)
        : imgui_layer("ember")
        , m_seed(0)
    {
#if OLD_SCENE
        m_2d_camera_controller =
            make_ref<orthographic_camera_controller>(
                glm::vec3{ 0.f,0.f,0.f }, width / height);
        m_scene_manager.add_scene(make_ref<noise1d_scene>(m_2d_camera_controller));
        m_scene_manager.add_scene(make_ref<noise2d_scene>(m_2d_camera_controller));
        m_scene_manager.add_scene(make_ref<roguelike_scene>(m_2d_camera_controller));
#endif
        m_viewport_size = { width, height };
        framebuffer_props props;
        props.width = static_cast<uint32_t>(width);
        props.height = static_cast<uint32_t>(height);
        m_framebuffer = frame_buffer_2d::create(props);
    }

    editor_layer::~editor_layer()
    {
    }


    void editor_layer::on_attach()
    {
        PYRO_PROFILE_FUNCTION();
        imgui_layer::on_attach();
#if OLD_SCENE
        m_scene_manager.init_first_scene();
#else
        m_active_scene = make_ref<scene>();

        auto green_square = m_active_scene->create_entity("Green Square");
        green_square.add_component<sprite_renderer_component>(glm::vec4{ 0.f,1.f,0.f,1.f });
        
        auto red_square = m_active_scene->create_entity("Red Square");
        red_square.add_component<sprite_renderer_component>(glm::vec4{ 1.f,0.f,0.f,1.f });
        
        m_camera_entity = m_active_scene->create_entity("Camera Entity");
        m_camera_entity.add_component<camera_component>();
        
        m_square_entity = green_square;

        m_second_camera = m_active_scene->create_entity("Clip-space Camera");
        auto &sc = m_second_camera.add_component<camera_component>();
        sc.primary = false;

        class camera_controller : public scriptable_entity
        {
        public:
            virtual void on_create() override
            {
            }

            virtual void on_update(timestep const &ts) override
            {
                auto &transform = get_component<transform_component>().transform;
                float speed = 5.0f;

                if(input::key_pressed(pyro::key_codes::KEY_A)) // left
                    transform[3][0] -= speed * ts;
                else if(input::key_pressed(pyro::key_codes::KEY_D)) // right
                    transform[3][0] += speed * ts;

                if(input::key_pressed(pyro::key_codes::KEY_W)) // up
                    transform[3][1] += speed * ts;
                else if(input::key_pressed(pyro::key_codes::KEY_S)) // down
                    transform[3][1] -= speed * ts;
            }

        };

        m_camera_entity.add_component<native_script_component>().bind<camera_controller>();

        // panels
        m_scene_hierarchy_panel.context(m_active_scene);
#endif
    }

    void editor_layer::on_detach()
    {
        PYRO_PROFILE_FUNCTION();
    }

    void editor_layer::on_update(timestep const &ts)
    {
        // Update
        PYRO_PROFILE_FUNCTION();

        if(m_viewport_size.x > 0.0f && m_viewport_size.y > 0.0f &&
            (m_framebuffer->width() != m_viewport_size.x
                || m_framebuffer->height() != m_viewport_size.y))
        {
            uint32_t width  = static_cast<uint32_t>(m_viewport_size.x);
            uint32_t height = static_cast<uint32_t>(m_viewport_size.y);
            m_framebuffer->resize(width, height);

#if OLD_SCENE
            m_2d_camera_controller->on_resize(m_viewport_size.x, m_viewport_size.y);
#else
            m_active_scene->on_viewport_resize(width, height);
#endif
        }
        if(m_viewport_focused)
        {
#if OLD_SCENE
            m_2d_camera_controller->on_update(ts);
#endif
        }
#if OLD_SCENE
        m_scene_manager.on_update(ts);
#else
        m_active_scene->on_update(ts);
#endif
    }

    void editor_layer::on_render() const
    {
        renderer_2d::reset_stats();
        
        m_framebuffer->bind();
        render_command::clear_color({ 0.1f, 0.1f, 0.1f, 1.f });
        render_command::clear();

#if OLD_SCENE
        m_scene_manager.on_render();
#else
        //renderer_2d::begin_scene(m_2d_camera_controller->camera());
        m_active_scene->on_render();
        //renderer_2d::end_scene();
#endif
        m_framebuffer->unbind();
        
    }

    void editor_layer::on_imgui_render()
    {
        // Note: Switch this to true to enable dockspace
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
        ImGui::Begin("Editor", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if(opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO &io = ImGui::GetIO();
        if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("DockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if(ImGui::BeginMenuBar())
        {
            if(ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows, 
                // which we can't undo at the moment without finer window depth/z control.
                //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                if(ImGui::MenuItem("Exit"))
                    application::instance().exit();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

#if OLD_SCENE
        auto current_scene = std::static_pointer_cast<base_noise_scene>(m_scene_manager.current_scene());

        // hide all ui if the scene is being played
        if(!current_scene->is_playing())
        {
            ImGui::Begin("Settings");

            auto stats = renderer_2d::stats();
            ImGui::Text("-- 2D Renderer stats:");
            float ms = application::frame_time() * 1000.f;
            ImGui::Text("- Frame time: %f ms", ms);
            ImGui::Text("- FPS: %d/s", application::fps());
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

        }
#else
        m_scene_hierarchy_panel.on_imgui_render();

        ImGui::Begin("Settings");
        if(m_square_entity)
        {
            {
                ImGui::Separator();
                auto &tag = m_square_entity.get_component<tag_component>().tag;
                ImGui::Text("%s", tag.c_str());

                auto &squareColor = m_square_entity.get_component<sprite_renderer_component>().color;
                ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
                ImGui::Separator();
            }
        }

        if(ImGui::Checkbox("Toggle Primary cameras", &m_is_primary_camera))
        {
            m_camera_entity.get_component<camera_component>().primary =  m_is_primary_camera;
            m_second_camera.get_component<camera_component>().primary = !m_is_primary_camera;
        }
#endif

        for(auto &result : m_profile_results)
        {
            char label[50];
            strcpy_s(label, "%.3fms ");
            strcat_s(label, result.name);
            ImGui::Text(label, result.time);
        }
        m_profile_results.clear();

        auto stats = renderer_2d::stats();
        ImGui::Text("-- 2D Renderer stats:");
        ImGui::Text("- Draw calls: %d", stats.draw_calls);
        ImGui::Text("- Quads: %d", stats.quad_count);
        ImGui::Text("- Vertices: %d", stats.total_vertex_count());
        ImGui::Text("- Indices: %d", stats.total_index_count());
        ImGui::Text("---------------------");
        ImGui::End();


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

        {
            ImGui::Begin("Viewport");

            m_viewport_focused = ImGui::IsWindowFocused();
            m_viewport_hovered = ImGui::IsWindowHovered();
            application::instance().gui_layer()->block_events(!m_viewport_focused || !m_viewport_hovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            m_viewport_size = { viewportPanelSize.x, viewportPanelSize.y };
            uint32_t textureID = m_framebuffer->color_attachment();
            ImGui::Image(reinterpret_cast<void *>(textureID), ImVec2{ m_viewport_size.x, m_viewport_size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
            ImGui::End();
        }
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void editor_layer::on_event(event &e)
    {
        imgui_layer::on_event(e);
#if OLD_SCENE
        m_2d_camera_controller->on_event(e);
#endif
        event_dispatcher dispatcher(e);
        // dispatch event on window X pressed 
#if OLD_SCENE
        auto current_scene = std::static_pointer_cast<base_noise_scene>(m_scene_manager.current_scene());
        dispatcher.dispatch<key_pressed_event>([&](key_pressed_event ev)
            {
                if(current_scene->is_playing() && ev.key_code() == key_codes::KEY_Q)
                {
                    current_scene->stop_playing();
                }
                // return if event is handled or not
                return false;
            });

        m_scene_manager.on_event(e);
#else
#endif
    }
}
