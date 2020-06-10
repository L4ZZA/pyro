#pragma once
#include <pyro.h>
#include <vector>
#include "utils/room.h"
#include "utils/random.h"

enum class e_tile_type
{
    floor = 0, wall, corridor, nothing
};

enum class e_room_type
{
    start = 0, anything, end
};

struct tile
{
    int x;
    int y;
    e_tile_type type;
    pyro::ref<room> room_ptr;

    bool is_floor(pyro::ref<room> r)
    {
        return x > r->left && x < r->right &&
            y > r->bottom && y < r->top;
    }

    bool is_wall(pyro::ref<room> r)
    {
        const bool is_left_wall = x == r->left && y >= r->bottom && y <= r->top;
        const bool is_right_wall = x == r->right && y >= r->bottom && y <= r->top;
        const bool is_top_wall = y == r->top && x >= r->left && x <= r->right;
        const bool is_bottom_wall = y == r->bottom && x >= r->left && x <= r->right;

        return is_left_wall || is_right_wall || is_top_wall || is_bottom_wall;
    }
};

class board_generator
{
public:
    board_generator(int width, int height);

    void on_update(pyro::timestep const &ts);
    void on_render() const;
    void on_imgui_render();
    void on_event(pyro::event &e);

    void init(int min_rooms, int max_rooms, int min_room_size, int max_room_size);
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
    pyro::ref<pyro::texture_2d> m_nothing_texture;
};