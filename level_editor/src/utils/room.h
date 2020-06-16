#pragma once
#include "utils/random.h"
#include "utils/door.h"
#include "utils/corridor.h"

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
        center.x = left   + static_cast<int>(width / 2.f);
        center.y = bottom + static_cast<int>(height / 2.f);
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

    bool is_centre(int x, int y) const
    {
        return x == center.x && y == center.y;
    }

    bool is_floor(int x, int y) const
    {
        return x > left && x < right &&
            y > bottom && y < top;
    }

    bool is_wall(int x, int y) const
    {
        const bool is_left_wall = x == left && y >= bottom && y <= top;
        const bool is_right_wall = x == right && y >= bottom && y <= top;
        const bool is_top_wall = y == top && x >= left && x <= right;
        const bool is_bottom_wall = y == bottom && x >= left && x <= right;

        return is_left_wall || is_right_wall || is_top_wall || is_bottom_wall;
    }

    bool is_door(int x, int y) const
    {
        const bool is_in_door = x == in_door.x && y == in_door.y;
        const bool is_out_door = x == out_door.x && y == out_door.y;

        return is_in_door || is_out_door;
    }

    bool is_in_door(int x, int y) const
    {
        const bool is_in_door = x == in_door.x && y == in_door.y;
        return is_in_door;
    }

    bool is_out_door(int x, int y) const
    {
        const bool is_out_door = x == out_door.x && y == out_door.y;
        return is_out_door;
    }

    bool are_overlapping(pyro::ref<room> other) const
    {
        const bool overlapping = left <= other->right
            && right >= other->left
            && top >= other->bottom
            && bottom <= other->top;
        return overlapping;
    }
    bool are_touching(pyro::ref<room> other) const
    {
        const int gap = 1;
        const bool touching = left == other->right + gap
            || right == other->left - gap
            || top == other->bottom - gap
            || bottom == other->top + gap;
        return touching;
    }
    bool are_near(pyro::ref<room> other) const
    {
        const int gap = 3;

        int gap_up = other->bottom - top;
        int gap_down = bottom - other->top;
        int gap_left = left - other->right;
        int gap_right = other->left - right;

        const bool a = gap_up >= 0 && gap_up <= gap;
        const bool b = gap_down >= 0 && gap_down <= gap;
        const bool c = gap_left >= 0 && gap_left <= gap;
        const bool d = gap_right >= 0 && gap_right <= gap;
        const bool are_near = a || b || c || d;
        return are_near;
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