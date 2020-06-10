#pragma once
#include "base_noise_scene.h"
#include "utils/random.h"
#include "utils/board_generator.h"
#include <vector>
#include <unordered_map>

class roguelike_scene final : public base_noise_scene
{
public:
    roguelike_scene(pyro::ref<pyro::camera_controller> cam_controller);
    virtual ~roguelike_scene();

    void init() override;
    void deinit() override;
    void on_update(pyro::timestep const &ts) override;
    void on_render() const override;
    void on_imgui_render() override;
    void on_event(pyro::event &e) override;

private:
    void on_seed_changed() override;
    bool on_key_pressed(pyro::key_pressed_event &e);
    glm::vec4 color_map(float noise) const;

private:
    pyro::ref<pyro::camera_controller> m_cam_controller;
    
    int                 m_seed;
    utils::random       m_rand;
    utils::perlin_noise m_other_noise;

    bool m_noise_changed = false;
    board_generator m_board_generator;
    //std::unordered_map<uint32_t, pyro::ref<room>> m_rooms_map;
    //map_generator m_map_gen;
};