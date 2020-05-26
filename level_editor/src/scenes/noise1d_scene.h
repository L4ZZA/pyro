#pragma once
#include "base_noise_scene.h"
#include "utils/random.h"

class noise1d_scene final : public base_noise_scene
{
public:
    noise1d_scene(pyro::ref<pyro::camera_controller> cam_controller);
    virtual ~noise1d_scene();

    void init() override;
    void deinit() override;
    void on_update(pyro::timestep const &ts) override;
    void on_render_internal() const override;
    void on_imgui_render() override;
    void on_event(pyro::event &e) override;

private:
    void on_seed_changed() override;
    bool on_key_pressed(pyro::key_pressed_event &e);
    glm::vec4 color_map(float noise) const;

private:
    pyro::ref<pyro::camera_controller> m_cam_controller;
    
    static const int s_texture_size = 256;
    pyro::ref<pyro::texture_2d> m_noise_texture;

    std::array<float, s_texture_size> m_noise_1d{ 0 };

    utils::random m_rand;
    int m_seed;
    bool m_noise_changed = false;
    int m_octaves = 5;
    float m_bias = 0.2f;

    float rect_width = 1.f;
};