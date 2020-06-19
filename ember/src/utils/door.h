#pragma once

enum class e_orientation
{
    north = 0, east, west, south
};

struct door
{
    int x;
    int y;
    // direction in which the door is facing, so on which wall is placed.
    e_orientation direction;
    bool open = false;
    bool is_the_same(door const &other) const
    {
        return x == other.x
            && y == other.y;
    }
};