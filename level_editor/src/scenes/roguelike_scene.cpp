#include "scenes/roguelike_scene.h"
#include "imgui/imgui.h"
#include "utils/noise.h"

// -------------------------------------------------------------------

roguelike_scene::roguelike_scene(pyro::ref<pyro::camera_controller> cam_controller)
    : base_noise_scene(cam_controller->camera())
    , m_cam_controller(cam_controller)
    , m_seed(7)
    , m_rand(0)
    , m_other_noise(0)
{
}

roguelike_scene::~roguelike_scene()
{
}

bool is_floor(room const &r, int x, int y)
{
    return x > r.pos_x && x < r.pos_x + r.width -1 &&
        y > r.pos_y && y < r.pos_y + r.height - 1;
}

bool is_wall(room const &r, int x, int y)
{
    return x >= r.pos_x && x < r.pos_x + r.width &&
        y >= r.pos_y && y < r.pos_y + r.height;
}

bool are_overlapping(room const &a, room const &b)
{
    glm::vec2 min(a.pos_x, a.pos_y);
    glm::vec2 max(a.pos_x + a.width, a.pos_y + a.height);
    glm::vec2 other_min(b.pos_x,  b.pos_y);
    glm::vec2 other_max(b.pos_x + b.width, b.pos_y + b.height);

    bool overlapping = (max >= other_min && min < other_max) 
                    || (min <= other_max && max < other_min);
    return overlapping;
}


void roguelike_scene::init()
{
    m_cam_controller->position({ 39.f, 24.f, 0.f });
    m_cam_controller->zoom_level(29.f);

    m_wall_texture  = pyro::texture_2d::create_from_file("assets/textures/wall.png");
    m_floor_texture = pyro::texture_2d::create_from_file("assets/textures/floor.png");
    m_nothing_texture   = pyro::texture_2d::create_from_file("assets/textures/nothing.png");

    size_t size = m_width * m_height;
    m_tiles.resize(size);

    on_seed_changed();
}

void roguelike_scene::deinit()
{
}

void roguelike_scene::on_update(pyro::timestep const &ts)
{
    if(m_noise_changed)
    {
        m_noise_changed = false;

        int n_rooms = m_rand.get_int(4, 10);
        m_rooms.resize(n_rooms);

        int i = 0;
        for(int i = 0; i < n_rooms; i++)
        {
            // create random room
            room rr;
            rr.width = m_rand.get_int(4, 8);
            rr.height = m_rand.get_int(4, 8);
            rr.pos_x = m_rand.get_int(0, m_width - rr.width);
            rr.pos_y = m_rand.get_int(0, m_height - rr.height);
            //rr.pos_x = width / 2.f  - rr.width / 2.f;
            //rr.pos_y = height / 2.f - rr.height / 2.f;

            // go through all the previously created rooms if any
            int j = 0;
            while(j < i)
            {
                // if the new room isn't overlapping with any other keep it 
                bool overlapping = are_overlapping(rr, m_rooms[j]);
                
                if(overlapping)
                {
                    // change room
                    rr.width = m_rand.get_int(4, 8);
                    rr.height = m_rand.get_int(4, 8);
                    rr.pos_x = m_rand.get_int(0, m_width - rr.width);
                    rr.pos_y = m_rand.get_int(0, m_height - rr.height);
                    j = 0;
                }
                else
                {
                    // check against next previously created room
                    j++;
                }
            }
            m_rooms[i] = rr;
            i++;
        }


        for(float x = 0; x < m_width; x++)
            for(float y = 0; y < m_height; y++)
            {
                int index = x * m_height + y;
                pyro::quad_properties props;
                //props.size = glm::vec2(0.95f);
                props.position = { x, y, 0.f };
                for(auto &room : m_rooms)
                {
                    props.texture = m_nothing_texture;
                    bool room_found = false;
                    if(is_wall(room, x, y))
                    {
                        props.texture = m_wall_texture;
                        room_found = true;
                    }
                    if(is_floor(room, x, y))
                    {
                        props.texture = m_floor_texture;
                        room_found = true;
                    }
                    if(room_found)
                        break;
                }
                m_tiles[index].props = props;
            }
    //    if(m_octaves > 8)
    //        m_octaves = 8;
    //    if(m_octaves < 1)
    //        m_octaves = 1;
    //    if(m_bias < 0.2f)
    //        m_bias = 0.2f;

    //    if(m_noise_type == 0)
    //    {
    //        utils::perlin_noise_2d(
    //            s_texture_size, m_octaves, m_bias, m_seed, m_noise_2d.data());

    //    }
    //    else if(m_noise_type == 1)
    //    {
    //        auto tmp_vec = m_other_noise.noise_2d_array(
    //            s_texture_size,
    //            m_scale,
    //            m_morph,
    //            m_move_x, m_move_y);
    //        std::copy_n(std::move(tmp_vec.begin()), tmp_vec.size(), m_noise_2d.begin());

    //    }
    //    m_noise_texture->data(m_noise_2d.data(), m_noise_2d.size(),
    //        pyro::e_texture_data_type::Float);

    }
}


void roguelike_scene::on_render_internal() const
{
    // create first room roughly in the middle of the 
    for(auto const &tile : m_tiles)
        pyro::renderer_2d::draw_quad(tile.props);

        
}

void roguelike_scene::on_imgui_render()
{
    //const std::array<char *, 2> items = { "Simple Noise", "Improved Perlin" };
    //static const char *current_item = "Simple Noise";

    ImGui::Text("- Seed: %d", m_seed);

    ImGui::Text("-- Rooms: ");
    ImGui::Text("- Count : %d", m_rooms.size());
    //ImGui::SameLine();
    //// The second parameter is the label previewed before opening the combo.
    //if(ImGui::BeginCombo("##combo", current_item))
    //{
    //    for(int n = 0; n < items.size(); n++)
    //    {
    //        // You can store your selection however you want, outside or inside your objects
    //        bool is_selected = (current_item == items[n]);
    //        if(ImGui::Selectable(items[n], is_selected))
    //        {
    //            if(m_noise_type != n)
    //            {
    //                m_noise_type = n;
    //                on_seed_changed();
    //                current_item = items[n];
    //            }
    //            //if(is_selected)
    //            //{
    //                //// You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
    //                //ImGui::SetItemDefaultFocus();   
    //            //}
    //        }
    //    }
    //    ImGui::EndCombo();
    //}
    //ImGui::Text("- Seed: ");
    //ImGui::SameLine();
    //if(ImGui::InputInt("##seed", &m_seed))
    //{
    //    on_seed_changed();
    //}

    //if(m_noise_type == 0)
    //{
    //    ImGui::Text("- Octaves: ");
    //    ImGui::SameLine();
    //    m_noise_changed |= ImGui::SliderInt("##octaves", &m_octaves, 1, 8);
    //    ImGui::Text("- Bias: ");
    //    ImGui::SameLine();
    //    m_noise_changed |= ImGui::SliderFloat("##bias", &m_bias, 0.1f, 2.f);
    //}
    //else if(m_noise_type == 1)
    //{
    //    ImGui::Text("- Scale: ");
    //    ImGui::SameLine();
    //    m_noise_changed |= ImGui::SliderInt("##scale", &m_scale, 1, 100);
    //    ImGui::Text("- Morph: ");
    //    ImGui::SameLine();
    //    m_noise_changed |= ImGui::SliderFloat("##morph", &m_morph, 0.1f, 50.f);
    //    ImGui::Text("- Move x: ");
    //    ImGui::SameLine();
    //    m_noise_changed |= ImGui::SliderFloat("##move_x", &m_move_x, -50.f, 50.f);
    //    ImGui::Text("- Move y: ");
    //    ImGui::SameLine();
    //    m_noise_changed |= ImGui::SliderFloat("##move_y", &m_move_y, -50.f, 50.f);
    //}

}

void roguelike_scene::on_event(pyro::event &e)
{
    pyro::event_dispatcher dispatcher(e);
    dispatcher.dispatch<pyro::key_pressed_event>(BIND_EVENT_FN(roguelike_scene::on_key_pressed));
}


bool roguelike_scene::on_key_pressed(pyro::key_pressed_event &e)
{
    if(e.key_code() == pyro::key_codes::KEY_KP_ADD)
    {
        m_seed++;
        on_seed_changed();
    }
    else if(e.key_code() == pyro::key_codes::KEY_KP_SUBTRACT)
    {
        m_seed--;
        on_seed_changed();
    }
    
    //if(e.key_code() == pyro::key_codes::KEY_DOWN)
    //{
    //    m_octaves--;
    //    m_noise_changed = true;
    //}
    //else if(e.key_code() == pyro::key_codes::KEY_UP)
    //{
    //    m_octaves++;
    //    m_noise_changed = true;
    //}
    //if(e.key_code() == pyro::key_codes::KEY_LEFT)
    //{
    //    if(m_bias > 0.2f)
    //    {
    //        m_bias -= 0.2f;
    //        m_noise_changed = true;
    //    }
    //}
    //else if(e.key_code() == pyro::key_codes::KEY_RIGHT)
    //{
    //    m_bias += 0.2f;
    //    m_noise_changed = true;
    //}


    //PYRO_TRACE("{0}", static_cast<char>(e.key_code())); 
    
    return false;
}

glm::vec4 roguelike_scene::color_map(float noise) const
{
    glm::vec4 color;

    // blue
    color = { 0.1f, 0.1f, 0.8, 1.0f };
    // light blue
    if(noise > 0.1f)
        color = { 0.3f, 0.3, 0.9f, 1.0f };
    // green
    if(noise > 0.25f)
        color = { 0.2f, 0.8, 0.2f, 1.0f };
    // light brown
    if(noise > 0.45f)
        color = { .46f, 0.35f, 0.3f, 1.0f };
    // brown
    if(noise > 0.65f)
        color = { .36f, 0.25f, 0.2f, 1.0f };
    // white
    if(noise > 0.85f)
        color = { 1.f, 1.f, 1.f, 1.0f };

    return color;
}

void roguelike_scene::on_seed_changed()
{
    m_rand.seed(m_seed);
    m_other_noise.change_seed(m_rand.seed());
    m_noise_changed = true;
}
