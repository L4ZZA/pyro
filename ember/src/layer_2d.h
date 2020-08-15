#pragma once 
#include "pyro.h"
#include "scenes/base_noise_scene.h"
#include "scenes/scene_manager.h"
#include "utils/perlin_noise.h" 


class layer_2d final : public pyro::imgui_layer
{

public:
    layer_2d(float width, float height);
    ~layer_2d() override;
    void on_attach() override;
    void on_detach() override;
    void on_update(const pyro::timestep &ts) override;
    void on_render() const override;
    void on_imgui_render() override;
    void on_event(pyro::event &e) override;

private:
    pyro::ref<pyro::camera_controller> m_2d_camera_controller;
    scene_manager m_scene_manager;
    pyro::ref<pyro::frame_buffer> m_framebuffer;
    glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

    int32_t m_seed;
    
    struct profile_result
    {
        char const *name;
        float time;
    };
    std::vector<profile_result> m_profile_results;
};
