#pragma once
#include "base_noise_scene.h"
#include "utils/random.h"

class noise2d_scene final : public base_noise_scene
{
public:
    noise2d_scene(pyro::ref<pyro::camera_controller> cam_controller);
    virtual ~noise2d_scene();

    void init() override;
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
    bool on_key_pressed(pyro::key_pressed_event &e);
    int color_map(float noise) const;

private:
    pyro::ref<pyro::camera_controller> m_cam_controller;
    bool m_play_mode;
    bool m_show_texture;
    bool m_textured_tiles;

    int   m_width;
    int   m_height;

    utils::random m_rand;
    int m_noise_type = 0;
    int m_seed;

    pyro::ref<pyro::texture_2d> m_noise_texture;
    std::vector<glm::vec4>      m_bg_colors;
    std::vector<pyro::ref<pyro::texture_2d>> m_bg_textures;
    std::vector<float> m_noise_2d;

    int m_scale = 10;
    float m_morph = 0.8f;
    float m_move_x = 0.8f;
    float m_move_y = 0.8f;

    bool m_noise_changed = false;
    bool m_seed_changed = false;
    int m_octaves = 5;
    float m_bias = 0.2f;

    utils::perlin_noise             m_other_noise;
    const float                     m_rect_speed{ 1.f };
};