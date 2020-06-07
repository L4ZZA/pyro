#include "board_generator.h"
#include "imgui/imgui.h"


board_generator::board_generator(int width, int height)
    : m_width(width)
    , m_height(height)
    , m_min_rooms(20)
    , m_max_rooms(35)
    , m_min_room_size(6)
    , m_max_room_size(12)
{
}

void board_generator::init(
    int min_rooms, int max_rooms, 
    int min_room_size, int max_room_size)
{
    m_min_rooms = min_rooms;
    m_max_rooms = max_rooms;
    m_min_room_size = min_room_size;
    m_max_room_size = max_room_size;
    m_tiles.resize(m_width * m_height);

    m_wall_texture = pyro::texture_2d::create_from_file("assets/textures/wall.png");
    m_floor_texture = pyro::texture_2d::create_from_file("assets/textures/floor.png");
    m_nothing_texture = pyro::texture_2d::create_from_file("assets/textures/nothing.png");
}

void board_generator::create_room(utils::random &rand)
{
    m_rooms.resize(0);
    m_possible_rooms = rand.get_int(m_min_rooms, m_max_rooms);

    // create random room
    auto first_room = pyro::make_ref<room>();
    first_room->width = rand.get_int(m_min_room_size, m_max_room_size);
    first_room->height = rand.get_int(m_min_room_size, m_max_room_size);
    first_room->pos_x = rand.get_int(0, m_width - first_room->width);
    first_room->pos_y = rand.get_int(0, m_height - first_room->height);
    m_rooms.push_back(first_room);

    for(int i = 1; i < m_possible_rooms; i++)
    {
        // create random room
        auto proposed_room = pyro::make_ref<room>();
        proposed_room->width = rand.get_int(4, 8);
        proposed_room->height = rand.get_int(4, 8);
        proposed_room->pos_x = rand.get_int(0, m_width - proposed_room->width);
        proposed_room->pos_y = rand.get_int(0, m_height - proposed_room->height);

        // go through all the previously created rooms to check if 
        // the proposed_room overlaps with any of them.
        // only add if it doesn't
        if(!is_any_overlapping_or_touching(proposed_room))
        {
            m_rooms.push_back(proposed_room);

            PYRO_TRACE("----- room");
            PYRO_TRACE("x - {}", proposed_room->pos_x);
            PYRO_TRACE("y - {}", proposed_room->pos_y);
            PYRO_TRACE("width - {}", proposed_room->width);
            PYRO_TRACE("height - {}", proposed_room->height);
        }
    }


    for(float x = 0; x < m_width; x++)
        for(float y = 0; y < m_height; y++)
        {
            int index = x * m_height + y;
            tile &current_tile = m_tiles[index];
            current_tile.type = e_tile_type::nothing;
            current_tile.x = x;
            current_tile.y = y;
            for(auto const &r : m_rooms)
            {
                bool room_found = false;
                if(current_tile.is_wall(r))
                {
                    current_tile.type = e_tile_type::wall;
                    room_found = true;
                }
                if(current_tile.is_floor(r))
                {
                    current_tile.type = e_tile_type::floor;
                    room_found = true;
                }
                // stop looking for other rooms
                if(room_found)
                {
                    current_tile.room_ptr = r;
                    break;
                }
            }
        }
}

void board_generator::create_room(
    utils::random &rand, 
    std::vector<pyro::ref<room>> const &other_rooms)
{

}

void board_generator::on_update(pyro::timestep const &ts)
{
}

void board_generator::on_render() const
{
    // create first room roughly in the middle of the 
    for(auto const &tile : m_tiles)
    {
        pyro::quad_properties props;
        props.position = { tile.x, tile.y, 0 };
        props.size = glm::vec2(0.75f);
        switch(tile.type)
        {
        case e_tile_type::wall:
            props.texture = m_wall_texture;
            break;
        case e_tile_type::floor:
            props.texture = m_floor_texture;
            break;
        case e_tile_type::nothing:
            props.texture = m_nothing_texture;
            break;
        }
        pyro::renderer_2d::draw_quad(props);
    }
}

void board_generator::on_imgui_render()
{
    ImGui::Text("-- Rooms: ");
    ImGui::Text("- Count : %d", m_rooms.size());
    ImGui::Text("---------------------");
}

void board_generator::on_event(pyro::event &e)
{
}


bool board_generator::are_overlapping(pyro::ref<room> room_a, pyro::ref<room> room_b) const
{
    const glm::vec2 min(room_a->pos_x, room_a->pos_y);
    const glm::vec2 max(room_a->pos_x + room_a->width - 1, room_a->pos_y + room_a->height - 1);
    const glm::vec2 other_min(room_b->pos_x, room_b->pos_y);
    const glm::vec2 other_max(room_b->pos_x + room_b->width - 1, room_b->pos_y + room_b->height - 1);

    const float left   = min.x; const float other_left   = other_min.x;
    const float right  = max.x; const float other_right  = other_max.x;
    const float top    = max.y; const float other_top    = other_max.y;
    const float bottom = min.y; const float other_bottom = other_min.y;

    const bool overlapping = left   <= other_right
                          && right  >= other_left
                          && top    >= other_bottom
                          && bottom <= other_top;
    return overlapping;
}

bool board_generator::are_touching(pyro::ref<room> room_a, pyro::ref<room> room_b) const
{
    glm::vec2 min(room_a->pos_x, room_a->pos_y);
    glm::vec2 max(room_a->pos_x + room_a->width - 1, room_a->pos_y + room_a->height - 1);
    glm::vec2 other_min(room_b->pos_x, room_b->pos_y);
    glm::vec2 other_max(room_b->pos_x + room_b->width - 1, room_b->pos_y + room_b->height - 1);
    
    const float left   = min.x; const float other_left   = other_min.x;
    const float right  = max.x; const float other_right  = other_max.x;
    const float top    = max.y; const float other_top    = other_max.y;
    const float bottom = min.y; const float other_bottom = other_min.y;
    
    const int gap = 1;
    const bool touching = left   == other_right + gap
                       || right  == other_left - gap
                       || top    == other_bottom - gap
                       || bottom == other_top + gap;
    return touching;
}

bool board_generator::is_any_overlapping(pyro::ref<room> r) const
{
    for(auto existing_room : m_rooms)
    {
        if(are_overlapping(r, existing_room))
            return true;
    }
    // none are overlapping
    return false;
}

bool board_generator::is_any_touching(pyro::ref<room> r) const
{
    for(auto existing_room : m_rooms)
    {
        if(are_touching(r, existing_room))
            return true;
    }
    // none are overlapping
    return false;
}

bool board_generator::is_any_overlapping_or_touching(pyro::ref<room> r) const
{
    for(auto const &existing_room : m_rooms)
    {
        if(are_overlapping(r, existing_room) 
           || are_touching(r, existing_room))
            return true;
    }
    // none are overlapping
    return false;
}