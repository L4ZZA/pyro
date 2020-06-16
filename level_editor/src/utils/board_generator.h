#pragma once
#include <pyro.h>
#include <vector>
#include "utils/tile.h"
#include "utils/random.h"
#include "utils/perlin_noise.h"

enum class e_room_type
{
    start = 0, anything, end
};

class board_generator
{
public:
    board_generator(int width, int height);

    void init(utils::random const &rand,
              int min_rooms, int max_rooms, 
              int min_room_size, int max_room_size);

    void on_update(pyro::timestep const &ts);
    void on_render() const;
    void on_imgui_render();
    void on_event(pyro::event &e);

    void clear_board();
    pyro::ref<room> create_room(utils::random const &rand);
    void connect_rooms(utils::random const &rand);
    int tile_map(float noise) const;

    // This method assumes that the room pass as parameter is not yet
    // in the list of existing rooms.
    bool is_any_overlapping(pyro::ref<room> r) const;
    // This method assumes that the room pass as parameter is not yet
    // in the list of existing rooms.
    bool is_any_touching(pyro::ref<room> r) const;
    // This method assumes that the room pass as parameter is not yet
    // in the list of existing rooms.
    bool is_any_overlapping_or_touching(pyro::ref<room> r) const;
    // This method assumes that the room pass as parameter is not yet
    // in the list of existing rooms.
    bool is_any_overlapping_or_near(pyro::ref<room> r) const;

private:
    float m_tiles_delay;
    float m_rooms_delay;
    float m_corridors_delay;
    float m_tiles_time = 0.f;
    float m_rooms_time = 0.f;
    float m_corridors_time = 0.f;
    int m_tiles_up_to = 0;
    int m_rooms_up_to = 0;
    int m_corridors_up_to = 0;
    int m_min_rooms;
    int m_max_rooms;
    int m_width;
    int m_height;
    int m_min_room_size;
    int m_max_room_size;
    int m_possible_rooms;
    bool m_show_rooms;
    bool m_delays_ended;

    utils::perlin_noise m_perlin_noise;
    std::vector<tile> m_tiles;
    std::vector<pyro::ref<room>> m_rooms;
    std::vector<pyro::ref<corridor>> m_corridors;

    pyro::ref<pyro::texture_2d> m_floor_texture;
    std::vector<pyro::ref<pyro::texture_2d>> m_bg_textures;
};