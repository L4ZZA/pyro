#pragma once
#include <pyro.h>
#include "utils/room.h"


enum class e_tile_type
{
    floor = 0, wall, in_door, out_door, nothing
};

struct tile
{
    int x;
    int y;
    e_tile_type type;
    pyro::ref<room> room_ptr;

    bool is_floor(pyro::ref<room> r) const
    {
        return x > r->left && x < r->right &&
            y > r->bottom && y < r->top;
    }

    bool is_wall(pyro::ref<room> r) const
    {
        const bool is_left_wall = x == r->left && y >= r->bottom && y <= r->top;
        const bool is_right_wall = x == r->right && y >= r->bottom && y <= r->top;
        const bool is_top_wall = y == r->top && x >= r->left && x <= r->right;
        const bool is_bottom_wall = y == r->bottom && x >= r->left && x <= r->right;

        return is_left_wall || is_right_wall || is_top_wall || is_bottom_wall;
    }

    bool is_door(pyro::ref<room> r) const
    {
        const bool is_in_door  = x == r->in_door.x && y == r->in_door.y;
        const bool is_out_door = x == r->out_door.x && y == r->out_door.y;

        return is_in_door || is_out_door;
    }

    bool is_in_door(pyro::ref<room> r) const
    {
        const bool is_in_door  = x == r->in_door.x && y == r->in_door.y;
        return is_in_door;
    }

    bool is_out_door(pyro::ref<room> r) const
    {
        const bool is_out_door = x == r->out_door.x && y == r->out_door.y;
        return is_out_door;
    }
};