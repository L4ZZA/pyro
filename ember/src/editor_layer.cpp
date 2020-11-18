#include "pyro.h"
#include "editor_layer.h"
#include "imgui/imgui.h"
#include "scenes/noise1d_scene.h" 
#include "scenes/noise2d_scene.h" 
#include "scenes/roguelike_scene.h"
#include "utils/random.h" 
#include "pyro/utils/platform_helpers.h" 

#include <glm/gtc/type_ptr.hpp>


namespace pyro
{
    editor_layer::editor_layer(uint32_t width, uint32_t height)
        : imgui_layer(width, height, "ember")
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
    }

    editor_layer::~editor_layer()
    {
    }


    void editor_layer::on_attach()
    {
        PYRO_PROFILE_FUNCTION();
        imgui_layer::on_attach();
        
        m_viewport_size = { static_cast<float>(m_layer_width), static_cast<float>(m_layer_height) };
        framebuffer_props props;
        props.width  = m_layer_width;
        props.height = m_layer_height;
        m_framebuffer = frame_buffer_2d::create(props);

#if OLD_SCENE
        m_scene_manager.init_first_scene();
#else
        m_active_scene = make_ref<scene>();
        
#if 0 
        const char* spritesheet_path = "assets/textures/RPGpack_sheet_2X.png";
        m_barrel_texture = pyro::sub_texture_2d::create_from_coords(spritesheet_path, { 8,2 }, {128.f, 128.f});
        m_tree_texture = pyro::sub_texture_2d::create_from_coords(spritesheet_path, { 2,1 }, { 128.f, 128.f }, { 1.f,2.f });

        auto green_square = m_active_scene->create_entity("Barrel", {1.f,0.f,0.f});
        green_square.add_component<sprite_renderer_component>(m_barrel_texture);
        
        auto red_square = m_active_scene->create_entity("Tree", {0.f,0.5f,0.f});
        red_square.add_component<sprite_renderer_component>(m_tree_texture);
        auto &scale = red_square.get_component<pyro::transform_component>().scale;
        scale = { 1.f,2.f,0.f };
        
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
                auto &translation = get_component<transform_component>().translation;
                float speed = 5.0f;

                if(input::key_pressed(pyro::key_codes::KEY_A)) // left
                    translation.x -= speed * ts;
                else if(input::key_pressed(pyro::key_codes::KEY_D)) // right
                    translation.x += speed * ts;

                if(input::key_pressed(pyro::key_codes::KEY_W)) // up
                    translation.y += speed * ts;
                else if(input::key_pressed(pyro::key_codes::KEY_S)) // down
                    translation.y -= speed * ts;
            }

        };

        m_camera_entity.add_component<native_script_component>().bind<camera_controller>();
#endif
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
        // setting min width for dock panels
        ImGuiStyle &style = ImGui::GetStyle();
        float min_win_size_x = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.f;
        if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("DockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        style.WindowMinSize.x = min_win_size_x;

        if(ImGui::BeginMenuBar())
        {
            if(ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows, 
                // which we can't undo at the moment without finer window depth/z control.
                //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
                if(ImGui::MenuItem("New", "Ctrl+N"))
					new_scene();

                if(ImGui::MenuItem("Open..", "Ctrl+O"))
                    open_scene();

                if(ImGui::MenuItem("Save As..", "Ctrl+Shift+S"))
                    save_scene_as();

                if(ImGui::MenuItem("Exit"))
                    application::instance().exit();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        
        
        ImGui::Begin("Stats");
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
        ImGui::End();

#if OLD_SCENE
        auto current_scene = std::static_pointer_cast<base_noise_scene>(m_scene_manager.current_scene());
        bool is_panel_visible = !current_scene->is_playing();

        // hide all ui if the scene is being played
        if(is_panel_visible)
        {
            ImGui::Begin("Settings");

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
        ImGui::End();
#else
        m_scene_hierarchy_panel.on_imgui_render();
#endif

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

        {
            ImGui::Begin("Viewport");

            m_viewport_focused = ImGui::IsWindowFocused();
            m_viewport_hovered = ImGui::IsWindowHovered();
            application::instance().gui_layer()->block_events(!m_viewport_focused || !m_viewport_hovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            m_viewport_size = { viewportPanelSize.x, viewportPanelSize.y };
            uint32_t textureID = m_framebuffer->color_attachment()->id();
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
		dispatcher.dispatch<key_pressed_event>(BIND_EVENT_FN(editor_layer::on_key_pressed));
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
    bool editor_layer::on_key_pressed(key_pressed_event &e)
    {
		// Shortcuts
		if (e.repeats_count() > 0)
			return false;

		bool control = input::key_pressed(key_codes::KEY_LEFT_CONTROL) || input::key_pressed(key_codes::KEY_RIGHT_CONTROL);
		bool shift = input::key_pressed(key_codes::KEY_LEFT_SHIFT) || input::key_pressed(key_codes::KEY_RIGHT_SHIFT);
		switch (e.key_code())
		{
        case key_codes::KEY_N:
			{
				if (control)
					new_scene();

				break;
			}
            case key_codes::KEY_O:
			{
				if (control)
					open_scene();

				break;
			}
			case key_codes::KEY_S:
			{
				if (control && shift)
					save_scene_as();

				break;
			}
		}
    }
    void editor_layer::new_scene()
    {
		m_active_scene = make_ref<scene>();
		m_active_scene->on_viewport_resize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
		m_scene_hierarchy_panel.context(m_active_scene);
    }

    void editor_layer::open_scene()
    {
		std::string filepath = FileDialogs::OpenFile("Pyro Scene (*.pyro)\0*.pyro\0");
        if(!filepath.empty())
        {
            new_scene();
			scene_serializer serializer(m_active_scene);
			serializer.deserialize(filepath);
        }
    }
    
    void editor_layer::save_scene_as()
    {
		std::string filepath = FileDialogs::SaveFile("Pyro Scene (*.pyro)\0*.pyro\0");
		if (!filepath.empty())
		{
			scene_serializer serializer(m_active_scene);
			serializer.serialize(filepath);
		}
    }
}
