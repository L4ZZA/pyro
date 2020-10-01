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
    glm::vec4 color_map(float noise) const;

private:
    pyro::ref<pyro::camera_controller> m_cam_controller;
    
    int                 m_board_width;
    int                 m_board_height;
    bool                m_play_mode;
    // Maximum amount of times that a room can take to be created.
    // if this number is met, no more rooms will be added to the m_rooms list.
    int                 m_max_room_tries;
    // The minimum amount of rooms to try create. 
    // [NOTE - It might not always get there as once the room couldn't 
    // be create for m_min_room_tries time the creation will stop]
    int                 m_min_rooms;
    int                 m_min_room_size;
    int                 m_max_room_size;
    int                 m_seed;
    utils::random       m_rand;
    utils::perlin_noise m_other_noise;

    bool m_noise_changed = false;
    board_generator m_board_generator;
    //std::unordered_map<uint32_t, pyro::ref<room>> m_rooms_map;
    //map_generator m_map_gen;
};