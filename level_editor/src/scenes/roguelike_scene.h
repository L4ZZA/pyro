#pragma once
#include "base_noise_scene.h"
#include "utils/random.h"
#include <vector>
#include <unordered_map>

enum class e_tile_type
{
    room = 0, wall, corridor, nothing
};

enum class e_room_type
{
    start = 0, anything, end
};

struct tile
{
    int pos_x;
    int pos_y;
    e_tile_type type;
    pyro::quad_properties props;
};

struct room
{
    int pos_x;
    int pos_y;
    int width;
    int height;
    
    bool is_the_same(room const &other) const
    {
        return pos_x  == other.pos_x 
            && pos_y == other.pos_y
            && width == other.width
            && height == other.height;
    }
};

inline bool operator==(room const &left, room const &right)
{
    return left.is_the_same(right);
}

inline bool operator!=(room const &left, room const &right)
{
    return !left.is_the_same(right);
}

class roguelike_scene final : public base_noise_scene
{
public:
    roguelike_scene(pyro::ref<pyro::camera_controller> cam_controller);
    virtual ~roguelike_scene();

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
    
    int                 m_seed;
    utils::random       m_rand;
    utils::perlin_noise m_other_noise;

    bool m_noise_changed = false;
    int max_rooms = 5;
    int m_width  = 80;
    int m_height = 50;
    pyro::ref<pyro::texture_2d> m_wall_texture;
    pyro::ref<pyro::texture_2d> m_floor_texture;
    pyro::ref<pyro::texture_2d> m_nothing_texture;
    std::vector<tile> m_tiles;
    std::vector<room>        m_rooms;
    std::unordered_map<uint32_t, room> m_rooms_map;
    //map_generator m_map_gen;
};