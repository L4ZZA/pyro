#pragma once
#include "pyro.h"
#include "utils/perlin_noise.h"

enum class e_noise_type
{
    one_d,
    two_d
};

class noise2d_scene final : public pyro::scene
{
public:
    noise2d_scene(pyro::ref<pyro::camera> const &camera);
    virtual ~noise2d_scene();

    void init();
    void on_update(pyro::timestep const &ts);
    void on_render_internal() const override;
    bool on_key_pressed(pyro::key_pressed_event &event);

private:
    glm::vec4 color_map(float noise) const;
    void reset_noise_seed(e_noise_type const &noise_type);

    int width = s_texture_size;
    int height = s_texture_size / 4;
    int step = 1;
    float line_rect_width = 0.2f;
    float rect_width = 1.f;
    float rect_heigth_max = 10.f;
    float gap_width = step - rect_width;
    float gap = 0.0f;

    pyro::ref<pyro::texture_2d> m_my_texture;
    static const int s_texture_size = 256;
    pyro::ref<pyro::texture_2d> m_noise_texture;

    std::array<float, s_texture_size> m_noise1d_seed{ 0 };
    std::array<float, s_texture_size> m_noise_1d{ 0 };

    std::array<float, s_texture_size *s_texture_size> m_noise2d_seed{ 0 };
    std::array<float, s_texture_size *s_texture_size> m_noise_2d{ 0 };
    std::array<float, s_texture_size *s_texture_size> m_vendor_noise_2d{ 0 };

    int m_seed;
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