﻿#pragma once
#include "pyro.h"

class layer_2d final : public pyro::imgui_layer
{
public:
    layer_2d();
    ~layer_2d() override;
    void on_attach() override;
    void on_detach() override;
    void on_update(const pyro::timestep& ts) override;
    void on_imgui_render() override;
    void on_event(pyro::event& event) override;
private:
    void reset_noise_seed();
    bool on_key_pressed(pyro::key_pressed_event& event);
private:
    pyro::orthographic_camera_controller m_2d_camera_controller; 
	
	// Temp
	pyro::ref<pyro::vertex_array> m_square_va;
	pyro::ref<pyro::vertex_array> m_rect_va;
	pyro::ref<pyro::shader> m_flat_color_shader;

	pyro::ref<pyro::texture_2d> m_checkerboard_texture;

	glm::vec4 m_square_color = { 0.2f, 0.3f, 0.8f, 1.0f };

    glm::vec3                       m_rect_color{.2f, .3f, .6f}; 
    glm::vec3                       m_rect_pos{0};
    float                           m_granularity = 0.1f;
    float                           m_x_count = 10;
    float                           m_y_count = 20;

    bool m_noise_changed = false;
    int m_octaves = 8;
    float m_bias = 0.2f;
    static const int s_texture_size = 512;
    std::array<float, s_texture_size> m_noise_seed{ 0 };
    std::array<float, s_texture_size> m_noise_1d{ 0 };


    const float                     m_rect_speed{1.f};

    struct profile_result
    {
        char const *name;
        float time;
    };
    std::vector<profile_result> m_profile_results;
};
