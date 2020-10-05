#include "board_generator.h"
#include "utils/imgui_extensions.h"


board_generator::board_generator()
    : m_width(1)
    , m_height(1)
    , m_possible_rooms(85)
    , m_tiles_delay(0.f)
    , m_rooms_delay(0.f)
    , m_corridors_delay(0.f)
    , m_show_dungeon(true)
    , m_show_walls(true)
    , m_delays_ended(false)
    , m_perlin_noise(0)
{
    m_floor_texture = pyro::texture_2d::create_from_file("assets/textures/stone-floor.png");
    m_bg_textures.resize(3);
    m_bg_textures[0] = pyro::texture_2d::create_from_file("assets/textures/lava.png");
    m_bg_textures[1] = pyro::texture_2d::create_from_file("assets/textures/nothing.png");
    m_bg_textures[2] = pyro::texture_2d::create_from_file("assets/textures/water.png");
}

void board_generator::init(
    utils::random const &rand,
    int width, int height,
    int min_rooms, int max_tries,
    int min_room_size, int max_room_size)
{
    m_width = width;
    m_height = height;
 
    // To be able to change the representation correctly
    // the seed will have to be dependent on other aspects
    // of the generation.
    int32_t combined_seed = rand.seed() 
                          + min_rooms + max_tries
                          + min_room_size + max_room_size;
    m_perlin_noise.change_seed(combined_seed);
    m_delays_ended = false;
    m_tiles_up_to = 0;
    m_rooms_up_to = 0;
    m_corridors_up_to = 0;

    clear_board();

    auto first_room = create_room(rand, min_room_size, max_room_size);
    m_rooms.push_back(first_room);

    int rooms_created = 1;
    int creation_tries = 0;
    // create rooms to meat the min rooms number
    while(true)
    {
        auto proposed_room = create_room(rand, min_room_size, max_room_size);

        // go through all the previously created rooms to check if 
        // the proposed_room overlaps with any of them.
        // only add if it doesn't
        if(!is_any_overlapping_or_near(proposed_room))
        {            
            m_rooms.push_back(proposed_room);
            rooms_created++;
            creation_tries = 0;
        }
        else
        {
            // stop from looping forever if there's no more space for more rooms.
            // or if the min rooms requirement was met.
            if(rooms_created >= min_rooms || creation_tries > max_tries)
            {
                break;
            }
            creation_tries++;
        }
    }

    connect_rooms();

    for(int x = 0; x < m_width; x++)
        for(int y = 0; y < m_height; y++)
        {
            int index = x * m_height + y;
            tile &current_tile = m_tiles[index];
            current_tile.x = x;
            current_tile.y = y;
            for(auto const &r : m_rooms)
            {
                bool room_found = false;
                if(r->is_wall(x,y) || r->is_floor(x,y))
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
    // logic for visualization effects
    if(!m_delays_ended)
    {
        if(m_tiles_delay > 0.f)
        {
            // draw tiles one by one in order of creation
            if(m_tiles_time > m_tiles_delay)
            {
                m_tiles_time = m_tiles_time - m_tiles_delay;
                if(m_tiles_up_to < m_tiles.size())
                    m_tiles_up_to++;
            }
            m_tiles_time += ts;
        }
        else
        {
            m_tiles_up_to = static_cast<int>(m_tiles.size());
        }

        if(m_rooms_delay > 0.f)
        {
            // draw rooms one by one in order of creation
            if(m_rooms_time > m_rooms_delay)
            {
                m_rooms_time = m_rooms_time - m_rooms_delay;
                if(m_rooms_up_to < m_rooms.size())
                    m_rooms_up_to++;
            }
            m_rooms_time += ts;
        }
        else
        {
            m_rooms_up_to = static_cast<int>(m_rooms.size());
        }

        if(m_corridors_delay > 0.f)
        {
            // draw corridors one by one in order of creation
            if(m_corridors_time > m_corridors_delay)
            {
                m_corridors_time = m_corridors_time - m_corridors_delay;
                if(m_corridors_up_to < m_corridors.size())
                    m_corridors_up_to++;
            }
            m_corridors_time += ts;
        }
        else
        {
            m_corridors_up_to = static_cast<int>(m_corridors.size());
        }

        m_delays_ended = m_tiles_up_to == m_tiles.size()
                       && m_rooms_up_to == m_rooms.size()
                       && m_corridors_up_to == m_corridors.size();
    }
}

void board_generator::on_render() const
{
    // create first room roughly in the middle of the 
    for(int i = 0; i < m_tiles_up_to; i++)
    {
        tile const &tile = m_tiles[i];
        pyro::quad_properties props;
        props.transform[3] = { tile.x, tile.y, 0.f, 0.f };
        float opacity = 0.75f;

        // the coordinates to the noise method have to be floating points values.
        float dx = static_cast<float>(tile.x) / (static_cast<float>(m_width));
        float dy = static_cast<float>(tile.y) / (static_cast<float>(m_height));

        float move_x = 0.f;
        float move_y = 0.f;
        float morph = 0.f;

        // to scale multiply by an integer
        int scale = 10;
        // Typical Perlin noise 
        float n = m_perlin_noise.noise(scale * dx - move_x, scale * dy + move_y, morph);

        props.texture = m_bg_textures[tile_map(n)];
        props.color = { 1.f,1.f,1.f,opacity };
        pyro::renderer_2d::draw_quad(props);
        
    }

    if(m_show_dungeon)
    {
        for(int i = 0; i < m_corridors_up_to; i++)
        {
            float opacity = 0.75f;
            for(int index : m_corridors[i]->tiles_indexes)
            {
                tile const &tile = m_tiles[index];
                pyro::quad_properties props;
                props.transform[3] = { tile.x, tile.y, 0.1f, 0.f };
                props.texture = m_floor_texture;
                //props.color = { 1.f,0.f,1.f,opacity };
                pyro::renderer_2d::draw_quad(props);
            }
        }

        for(int i = 0; i < m_rooms_up_to; i++)
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
                    props.transform[3] = { x, y, 0.1f, 0.f };
                    float opacity = 0.75f;
                    // only use floor texture if the tile is geometrically
                    // part of the floor (ignoring tile-type)
                    if(m_show_walls)
                    {
                        if(room->is_floor(x, y))
                        {
                            props.texture = m_floor_texture;
                        }
                    }
                    else
                    {
                        props.texture = m_floor_texture;
                    }
                    pyro::renderer_2d::draw_quad(props);
                }
        }
    }
}

void board_generator::on_imgui_render()
{
    ImGui::Text("--- Dungeon ");
    ImGui::Text("- Show dungeon "); 
    ImGui::ToggleButton("##dungeon", &m_show_dungeon);
    ImGui::Text("- Show walls "); 
    ImGui::ToggleButton("##walls", &m_show_walls);
    ImGui::Text("-- Change the following parameters to \n\
   see the order of creation.");
    ImGui::Text("-- Rooms: "); 
    if(ImGui::InputInt("##room_count", &m_rooms_up_to))
    {
        //also stop showing cooridors with room
        m_corridors_up_to = m_rooms_up_to - 1;
    }
    ImGui::Text("-- Corridors: "); 
    ImGui::InputInt("##corridor_count", &m_corridors_up_to);
    if(m_rooms_up_to < 0)
        m_rooms_up_to = 0;
    if(m_rooms_up_to > m_rooms.size())
        m_rooms_up_to = static_cast<int>(m_rooms.size());
    if(m_corridors_up_to < 0)
        m_corridors_up_to = 0;
    if(m_corridors_up_to > m_corridors.size())
        m_corridors_up_to = static_cast<int>(m_corridors.size());
    ImGui::Text("---------------------");
}

void board_generator::on_event(pyro::event &e)
{
}

void board_generator::clear_board()
{
    int size = m_width * m_height;
    m_tiles.resize(size);
    m_rooms.resize(0);
    m_corridors.resize(0);


    for(auto &tile : m_tiles)
    {
        tile.type = e_tile_type::nothing;
    }
}

pyro::ref<room> board_generator::create_room(
    utils::random const &rand, 
    int32_t min_size, 
    int32_t max_size)
{
    // create random room
    int gap = 3; // buffer around the edges of the world
    int width = rand.get_int (min_size, max_size);
    int height = rand.get_int(min_size, max_size);
    int x = rand.get_int(gap, m_width - width - gap);
    int y = rand.get_int(gap, m_height - height - gap);
    return std::move(pyro::make_ref<room>(x, y, width, height));
  
}

void board_generator::connect_rooms()
{
    m_corridors.resize(0);
    int max_corridors = static_cast<int>(m_rooms.size()) - 1;
    for(int i = 0; i < max_corridors; i++)
    {
        int next = i + 1;
        auto current_room = m_rooms[i];
        auto next_room = m_rooms[next];

        // get start door location (red-door) from first room
        glm::ivec2 start_tile(current_room->center.x, current_room->center.y);
        // get end door location (green-door) from next room
        glm::ivec2 dest_tile(next_room->center.x, next_room->center.y);
        
        // get the vector between going from the start doo to the dest door
        glm::ivec2 diff = dest_tile - start_tile;

        // store the offsets from which to start the corridor based on the 
        // displacement vector of the locations of the two doors
        int x_dir = diff.x < 0 ? -1 : 1;
        int y_dir = diff.y < 0 ? -1 : 1;

        // get units to travel
        glm::ivec2 displacement = glm::abs(diff);

        auto corr = pyro::make_ref<corridor>();
        corr->start_tile = start_tile;
        corr->end_tile = dest_tile;

        // if displacement is zero, the end_tile should 
        // be on the same x coordinate of the start_tile
        int end_tile = start_tile.x;
        for(int x = 0; x < displacement.x; x++)
        {
            int current_tile_x = start_tile.x + (x * x_dir);
            int current_tile_y = start_tile.y;
            int index = current_tile_x * m_height + current_tile_y;
            corr->tiles_indexes.push_back(index);
            end_tile = current_tile_x;
        }

        for(int y = 0; y < displacement.y; y++)
        {
            int current_tile_x = end_tile;
            int current_tile_y = start_tile.y + (y * y_dir);
            int index = current_tile_x * m_height + current_tile_y;
            corr->tiles_indexes.push_back(index);
        }

        // add corridor to list of corridors.
        m_corridors.push_back(corr);
    }
}

int board_generator::tile_map(float noise) const
{
    // water
    if(noise < 0.25f)
        return 2;
    // nothing
    if(noise < 0.65f)
        return 1;
    // lava
    return 0;
}

bool board_generator::is_any_overlapping(pyro::ref<room> r) const
{
    for(auto existing_room : m_rooms)
    {
        if(r->are_overlapping(existing_room))
            return true;
    }
    // none are overlapping
    return false;
}

bool board_generator::is_any_touching(pyro::ref<room> r) const
{
    for(auto existing_room : m_rooms)
    {
        if(r->are_touching(existing_room))
            return true;
    }
    // none are overlapping
    return false;
}

bool board_generator::is_any_overlapping_or_touching(pyro::ref<room> r) const
{
    for(auto const &existing_room : m_rooms)
    {
        if(r->are_overlapping(existing_room)
           || r->are_touching(existing_room))
            return true;
    }
    // none are overlapping
    return false;
}

bool board_generator::is_any_overlapping_or_near(pyro::ref<room> r) const
{
    for(auto const &existing_room : m_rooms)
    {
        if(r->are_overlapping(existing_room)
           || r->are_near(existing_room))
            return true;
    }
    // none are overlapping
    return false;
}