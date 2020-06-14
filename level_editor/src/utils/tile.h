#pragma once
#include <pyro.h>
#include "utils/room.h"


enum class e_tile_type
{
    floor = 0, wall, room_center, in_door, out_door, coridor_floor, nothing
};

struct tile
{
    int x;
    int y;
    e_tile_type type = e_tile_type::nothing;
    pyro::ref<room> room_ptr;
};