#include "board_generator.h"
#include "imgui/imgui.h"


board_generator::board_generator(int width, int height)
    : m_width(width)
    , m_height(height)
    , m_min_rooms(20)
    , m_max_rooms(35)
    , m_min_room_size(6)
    , m_max_room_size(12)
    , m_possible_rooms(0)
    , m_delay(1.f)
{
}

void board_generator::init(
    utils::random const &rand,
    int min_rooms, int max_rooms,
    int min_room_size, int max_room_size)
{
    up_to = 0;
    m_min_rooms = min_rooms;
    m_max_rooms = max_rooms;
    m_min_room_size = min_room_size;
    m_max_room_size = max_room_size;

    m_wall_texture = pyro::texture_2d::create_from_file("assets/textures/wall.png");
    m_floor_texture = pyro::texture_2d::create_from_file("assets/textures/floor.png");
    m_door_texture = pyro::texture_2d::create_from_file("assets/textures/door.png");
    m_nothing_texture = pyro::texture_2d::create_from_file("assets/textures/nothing.png");

    m_tiles.resize(m_width * m_height); m_rooms.resize(0);
    m_possible_rooms = rand.get_int(m_min_rooms, m_max_rooms);
    auto first_room = create_room(rand);
    m_rooms.push_back(first_room);

    for(int i = 1; i < m_possible_rooms; i++)
    {
        auto proposed_room = create_room(rand);

        // go through all the previously created rooms to check if 
        // the proposed_room overlaps with any of them.
        // only add if it doesn't
        if(!is_any_overlapping_or_near(proposed_room))
        {
            m_rooms.push_back(proposed_room);

            PYRO_TRACE("----- room");
            PYRO_TRACE("x - {}", proposed_room->left);
            PYRO_TRACE("y - {}", proposed_room->bottom);
            PYRO_TRACE("right - {}", proposed_room->right);
            PYRO_TRACE("top - {}", proposed_room->top);
            PYRO_TRACE("width - {}", proposed_room->width);
            PYRO_TRACE("height - {}", proposed_room->height);
        }
    }


    for(int x = 0; x < m_width; x++)
        for(int y = 0; y < m_height; y++)
        {
            int index = x * m_height + y;
            tile &current_tile = m_tiles[index];
            current_tile.type = e_tile_type::nothing;
            current_tile.x = x;
            current_tile.y = y;
            for(auto const &r : m_rooms)
            {
                bool room_found = false;
                if(is_wall(x,y,r))
                {
                    if(is_in_door(x,y,r))
                    {
                        current_tile.type = e_tile_type::in_door;
                    }
                    else if(is_out_door(x,y,r))
                    {
                        current_tile.type = e_tile_type::out_door;
                    }
                    else
                    {
                        current_tile.type = e_tile_type::wall;
                    }
                    room_found = true;
                }
                if(is_floor(x,y,r))
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

void board_generator::on_update(pyro::timestep const &ts)
{
    if(m_time > m_delay)
    {
        m_time = m_time - m_delay;
        if(up_to < m_rooms.size())
            up_to++;
    }

    m_time += ts;
}

void board_generator::on_render() const
{
    // create first room roughly in the middle of the 
    for(int i = 0; i < m_tiles.size(); i++)
    {
        tile const &tile = m_tiles[i];
        if(tile.type == e_tile_type::nothing)
        {
            pyro::quad_properties props;
            props.position = { tile.x, tile.y, 0 };
            props.texture = m_nothing_texture;
            float opacity = 0.75f;
            props.color = { 1.f,1.f,1.f,opacity };
            pyro::renderer_2d::draw_quad(props);
        }
    }

    for(int i = 0; i < up_to; i++)
    //for(int i = 0; i < m_rooms.size(); i++)
    {
        auto const &room = m_rooms[i];
        int x_start = room->left;
        int x_end = room->right;
        int y_start = room->bottom;
        int y_end = room->top;

        for(int x = x_start; x <= x_end; x++)
            for(int y = y_start; y <= y_end; y++)
            {
                pyro::quad_properties props;
                props.position = { x, y, 0 };
                //props.size = glm::vec2(0.75f);
                float opacity = 0.75f;
                if(is_wall(x, y, room))
                {
                    if(is_in_door(x, y, room))
                    {
                        props.texture = m_door_texture;
                        props.color = { 0.f,1.f,0.f,opacity };
                    }
                    else if(is_out_door(x, y, room))
                    {
                        props.texture = m_door_texture;
                        props.color = { 1.f,0.f,0.f,opacity };
                    }
                    else
                    {
                        props.texture = m_wall_texture;
                    }
                }
                else if(is_floor(x, y, room))
                {
                    props.texture = m_floor_texture;
                }
                pyro::renderer_2d::draw_quad(props);
            }
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

pyro::ref<room> board_generator::create_room(utils::random const &rand)
{
    // create random room
    int gap = 3; // buffer around the edges of the world
    int width = rand.get_int(m_min_room_size, m_max_room_size);
    int height = rand.get_int(m_min_room_size, m_max_room_size);
    int x = rand.get_int(gap, m_width - width - gap);
    int y = rand.get_int(gap, m_height - height - gap);
    return std::move(pyro::make_ref<room>(x, y, width, height));
  
}

bool board_generator::are_overlapping(pyro::ref<room> room_a, pyro::ref<room> room_b) const
{
    const bool overlapping = room_a->left   <= room_b->right
                          && room_a->right  >= room_b->left
                          && room_a->top    >= room_b->bottom
                          && room_a->bottom <= room_b->top;
    return overlapping;
}

bool board_generator::are_touching(pyro::ref<room> room_a, pyro::ref<room> room_b) const
{
    const int gap = 1;
    const bool touching = room_a->left   == room_b->right + gap
                       || room_a->right  == room_b->left - gap
                       || room_a->top    == room_b->bottom - gap
                       || room_a->bottom == room_b->top + gap;
    return touching;
}

bool board_generator::are_near(pyro::ref<room> room_a, pyro::ref<room> room_b) const
{
    const int gap = 3;
    const bool touching = room_a->left   < room_b->right + gap
                       || room_a->right  > room_b->left - gap
                       || room_a->top    > room_b->bottom - gap
                       || room_a->bottom < room_b->top + gap;
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

bool board_generator::is_any_overlapping_or_near(pyro::ref<room> r) const
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