#pragma once 
#include "pyro.h"
#include "scenes/base_noise_scene.h"
#include "scenes/scene_manager.h"
#include "utils/perlin_noise.h" 


namespace pyro
{
    class editor_layer final : public imgui_layer
    {

    public:
        editor_layer(float width, float height);
        ~editor_layer() override;
        void on_attach() override;
        void on_detach() override;
        void on_update(const timestep &ts) override;
        void on_render() const override;
        void on_imgui_render() override;
        void on_event(event &e) override;

    private:
        ref<camera_controller> m_2d_camera_controller;

#define OLD_SCENE 0
#if OLD_SCENE
        scene_manager m_scene_manager;
#else
        ref<scene> m_active_scene;
        entity m_square_entity;
#endif
        ref<frame_buffer> m_framebuffer;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;

        int32_t m_seed;

        struct profile_result
        {
            char const *name;
            float time;
        };
        std::vector<profile_result> m_profile_results;
    };
}
