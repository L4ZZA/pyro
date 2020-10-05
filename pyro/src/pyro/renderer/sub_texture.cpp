#include "pyro_pch.h"
#include "sub_texture.h"

pyro::sub_texture_2d::sub_texture_2d(
    ref<texture_2d> texture, 
    glm::vec2 const &min, 
    glm::vec2 const &max)
{
    m_texture = texture;
    m_texture_coords[0] = {min.x, min.y};
    m_texture_coords[1] = {max.x, min.y};
    m_texture_coords[2] = {max.x, max.y};
    m_texture_coords[3] = {min.x, max.y};
}

pyro::ref<pyro::sub_texture_2d> 
pyro::sub_texture_2d::create_from_coords(
    ref<texture_2d> texture, 
    glm::vec2 const &coords, 
    glm::vec2 const &cell_size,
    glm::vec2 const &sprite_size /*= { 1.f ,1.f }*/)
{
    const float sprite_width  = cell_size.x;
    const float sprite_height = cell_size.y;
    
    const float texture_width = static_cast<float>(texture->width());
    const float texture_height = static_cast<float>(texture->height());
    glm::vec2 min = { 
        (coords.x * sprite_width) / texture_width, 
        (coords.y * sprite_height) / texture_height 
    };
    glm::vec2 max = { 
        ((coords.x + sprite_size.x) * sprite_width) / texture_width, 
        ((coords.y + sprite_size.y) * sprite_height) / texture_height 
    };

    return make_ref<sub_texture_2d>(texture, min, max);
}
