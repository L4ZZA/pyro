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
    bool is_the_same(door const &other) const
    {
        return x == other.x
            && y == other.y;
            //&& direction == other.direction;
    }
};

struct corridor
{
    std::vector<int> tiles_indexes;
    glm::ivec2 start_tile;
    glm::ivec2 end_tile;
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
        center.x = right - ((right - left) / 2.f);
        center.y = top - ((top - bottom) / 2.f);
        generate_doors(rand, in_door, out_door);
    }

    int left;   // coordinate of the left wall
    int bottom; // coordinate of the bottom wall
    int right;  // coordinate of the right wall
    int top;    // coordinate of the top wall
    int width;
    int height;

    glm::ivec2 center;

    door out_door;
    door in_door;

    void generate_doors(utils::random const &rand, door &in, door &out)
    {
        in.direction = static_cast<e_orientation>(rand.get_int() % 4);
        out.direction = static_cast<e_orientation>(rand.get_int() % 4);
        fix_direction(rand, out.direction, in.direction);
        
        generate_location(rand, in.direction, in.x, in.y);
        generate_location(rand, out.direction, out.x, out.y);
    }

    // generates a new orientation until dir is different from the previous one.
    void fix_direction(
        utils::random const &rand, 
        e_orientation &dir, 
        e_orientation const &previous)
    {
        while(dir == previous)
        {
            int int_dir = (static_cast<int>(dir) + rand.get_int()) % 4;
            dir = static_cast<e_orientation>(int_dir);
        }
    }

    void generate_location(
        utils::random const &rand, 
        e_orientation dir, 
        int &x, int &y)
    {
        switch(dir)
        {
        case e_orientation::north:
            x = rand.get_int(left + 1, right - 1);
            y = top;
            break;
        case e_orientation::south:
            x = rand.get_int(left + 1, right - 1);
            y = bottom;
            break;
        case e_orientation::east:
            x = left;
            y = rand.get_int(bottom + 1, top - 1);
            break;
        case e_orientation::west:
            x = right;
            y = rand.get_int(bottom + 1, top - 1);
            break;
        }
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