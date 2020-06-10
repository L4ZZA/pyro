#pragma once


struct room
{
    room(int x, int y, int w, int h)
    {
        left   = x;
        bottom = y;
        right  = x + w-1;
        top    = y + h-1;
    }

    int left;   // coordinate of the left wall
    int bottom; // coordinate of the bottom wall
    int right;  // coordinate of the right wall
    int top;    // coordinate of the top wall

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