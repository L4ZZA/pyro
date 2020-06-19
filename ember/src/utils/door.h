#pragma once

enum class e_orientation
{
    north = 0, east, west, south
};

struct door
{
    int x = 0;
    int y = 0;
    // direction in which the door is facing, so on which wall is placed.
    e_orientation direction = e_orientation::north;
    bool open = false;
    bool is_the_same(door const &other) const
    {
        return x == other.x
            && y == other.y;
    }
};