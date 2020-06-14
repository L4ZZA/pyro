#pragma once
#include <pyro.h>
#include <vector>
#include "utils/tile.h"
#include "utils/random.h"

enum class e_room_type
{
    start = 0, anything, end
};

class board_generator
{
public:
    board_generator(int width, int height);

    void init(utils::random const &rand,
              int min_rooms, int max_rooms, 
              int min_room_size, int max_room_size);

    void on_update(pyro::timestep const &ts);
    void on_render() const;
    void on_imgui_render();
    void on_event(pyro::event &e);

    pyro::ref<room> create_room(utils::random const &rand);
    void connect_rooms(utils::random const &rand);

    // takes the direction of the room's door and it's position to adjust the position
    // to start from outside the room's walls.
    void adjust_corridor_end(e_orientation const &dir, glm::ivec2 &door_pos);

    bool are_overlapping(pyro::ref<room> room_a, pyro::ref<room> room_b) const;
    bool are_touching(pyro::ref<room> room_a, pyro::ref<room> room_b) const;
    bool are_near(pyro::ref<room> room_a, pyro::ref<room> room_b) const;
    bool is_any_overlapping(pyro::ref<room> r) const;
    bool is_any_touching(pyro::ref<room> r) const;
    bool is_any_overlapping_or_touching(pyro::ref<room> r) const;
    bool is_any_overlapping_or_near(pyro::ref<room> r) const;


    bool is_centre(int x, int y, pyro::ref<room> r) const
    {
        return false; //x == r->center.x && y == r->center.y;
    }

    bool is_floor(int x, int y, pyro::ref<room> r) const
    {
        return x > r->left && x < r->right &&
            y > r->bottom && y < r->top;
    }

    bool is_wall(int x, int y, pyro::ref<room> r) const
    {
        const bool is_left_wall = x == r->left && y >= r->bottom && y <= r->top;
        const bool is_right_wall = x == r->right && y >= r->bottom && y <= r->top;
        const bool is_top_wall = y == r->top && x >= r->left && x <= r->right;
        const bool is_bottom_wall = y == r->bottom && x >= r->left && x <= r->right;

        return is_left_wall || is_right_wall || is_top_wall || is_bottom_wall;
    }

    bool is_door(int x, int y, pyro::ref<room> r) const
    {
        const bool is_in_door = x == r->in_door.x && y == r->in_door.y;
        const bool is_out_door = x == r->out_door.x && y == r->out_door.y;

        return is_in_door || is_out_door;
    }

    bool is_in_door(int x, int y, pyro::ref<room> r) const
    {
        const bool is_in_door = x == r->in_door.x && y == r->in_door.y;
        return is_in_door;
    }

    bool is_out_door(int x, int y, pyro::ref<room> r) const
    {
        const bool is_out_door = x == r->out_door.x && y == r->out_door.y;
        return is_out_door;
    }

private:
    float m_tiles_delay;
    float m_rooms_delay;
    float m_corridors_delay;
    float m_tiles_time = 0.f;
    float m_rooms_time = 0.f;
    float m_corridors_time = 0.f;
    int m_tiles_up_to = 0;
    int m_rooms_up_to = 0;
    int m_corridors_up_to = 0;
    int m_min_rooms;
    int m_max_rooms;
    int m_width;
    int m_height;
    int m_min_room_size;
    int m_max_room_size;
    int m_possible_rooms;
    bool m_show_rooms;

    std::vector<tile> m_tiles;
    std::vector<pyro::ref<room>> m_rooms;
    std::vector<pyro::ref<corridor>> m_corridors;

    pyro::ref<pyro::texture_2d> m_wall_texture;
    pyro::ref<pyro::texture_2d> m_floor_texture;
    pyro::ref<pyro::texture_2d> m_door_texture;
    pyro::ref<pyro::texture_2d> m_nothing_texture;
};