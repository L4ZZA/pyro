#pragma once
#include <pyro.h>
#include <vector>
#include "utils/tile.h"
#include "utils/random.h"

enum class e_room_type
{
    start = 0, anything, end
};

class board_generator
{
public:
    board_generator(int width, int height);

    void init(int min_rooms, int max_rooms, int min_room_size, int max_room_size);

    void on_update(pyro::timestep const &ts);
    void on_render() const;
    void on_imgui_render();
    void on_event(pyro::event &e);

    void create_room(utils::random &rand);

    bool are_overlapping(pyro::ref<room> room_a, pyro::ref<room> room_b) const;
    bool are_touching(pyro::ref<room> room_a, pyro::ref<room> room_b) const;
    bool is_any_overlapping(pyro::ref<room> r) const;
    bool is_any_touching(pyro::ref<room> r) const;
    bool is_any_overlapping_or_touching(pyro::ref<room> r) const;

private:
    int m_min_rooms;
    int m_max_rooms;
    int m_width;
    int m_height;
    int m_min_room_size;
    int m_max_room_size;
    int m_possible_rooms;

    std::vector<tile> m_tiles;
    std::vector<pyro::ref<room>> m_rooms;

    pyro::ref<pyro::texture_2d> m_wall_texture;
    pyro::ref<pyro::texture_2d> m_floor_texture;
    pyro::ref<pyro::texture_2d> m_door_texture;
    pyro::ref<pyro::texture_2d> m_nothing_texture;
};