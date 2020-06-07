#pragma once


struct room
{
    int pos_x;
    int pos_y;
    int width;
    int height;



    bool is_the_same(room const &other) const
    {
        return pos_x == other.pos_x
            && pos_y == other.pos_y
            && width == other.width
            && height == other.height;
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