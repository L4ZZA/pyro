#pragma once 
#include "pyro.h" 
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
    pyro::orthographic_camera_controller m_2d_camera_controller;

    int32_t m_current_scene_index = 0;
    std::vector<pyro::ref<pyro::scene>> m_scenes;

    struct profile_result
    {
        char const *name;
        float time;
    };
    std::vector<profile_result> m_profile_results;
};
