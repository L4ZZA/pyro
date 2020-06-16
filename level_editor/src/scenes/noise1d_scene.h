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
    void on_render() const override;
    void on_imgui_render() override;
    void on_event(pyro::event &e) override;
    virtual void play() override { m_play_mode = true; }
    virtual void stop_playing() override { m_play_mode = false; }
    virtual bool is_playing() override { return m_play_mode; }

private:
    void on_seed_changed() override;

private:
    void editor_update(pyro::timestep const &ts);
    void play_mode_update(pyro::timestep const &ts);
    void create_map();
    bool on_key_pressed(pyro::key_pressed_event &e);
    glm::vec4 color_map(float noise) const;

private:
    pyro::ref<pyro::camera_controller> m_cam_controller;
    bool m_play_mode;

    int m_map_width;
    int m_map_height;
    static const int s_texture_size = 256;

    std::vector<float> m_noise_1d;
    std::vector<unsigned char> m_surface;

    utils::random m_rand;
    int m_noise_type = 0;
    int m_seed;
    bool m_noise_changed = false;
    int m_octaves = 5;
    float m_bias = 0.2f;

    int m_scale = 10;
    float m_morph = 0.8f;
    float m_move_x = 0.8f;

    utils::perlin_noise             m_other_noise;
    float rect_width = 1.f;
};