#pragma once
#include "utils/random.h"

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
};

struct room
{
    room(int x, int y, int w, int h)
    {
        utils::random rand(x+y+w+h);
        left   = x;
        bottom = y;
        right  = x + w-1;
        top    = y + h-1;
        width  = w;
        height = h;
        in_door = generate_door(rand);
        out_door = generate_door(rand);
        //sort_doors();
    }

    int left;   // coordinate of the left wall
    int bottom; // coordinate of the bottom wall
    int right;  // coordinate of the right wall
    int top;    // coordinate of the top wall
    int width;
    int height;

    door out_door;
    door in_door;

    door generate_door(utils::random &rand)
    {
        auto direction = static_cast<e_orientation>(rand.get_int() % 4);

        door d;
        d.direction = direction;

        switch(direction)
        {
        case e_orientation::north:
            d.x = rand.get_int(left + 1, right - 1);
            d.y = top;
            break;
        case e_orientation::south: 
            d.x = rand.get_int(left + 1, right - 1);
            d.y = bottom;
            break;
        case e_orientation::east:
            d.x = left;
            d.y = rand.get_int(bottom + 1, top - 1);
            break;
        case e_orientation::west:
            d.x = right;
            d.y = rand.get_int(bottom + 1, top - 1);
            break;
        }

        return std::move(d);
    }

    bool is_the_same(room const &other) const
    {
        return left == other.left
            && bottom == other.bottom
            && right == other.right
            && top == other.top;
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