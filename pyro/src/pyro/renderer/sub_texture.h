#pragma once
#include "pyro/renderer/texture.h"
#include <glm/glm.hpp>

namespace pyro
{
    // Represents a portion of texture based on the given min and max corners.
    // min - bottom left coordinate
    class sub_texture_2d
    {
    public:
        sub_texture_2d(
            ref<texture_2d> texture,
            glm::vec2 const &min,
            glm::vec2 const &max
            );

        ref<texture_2d> const& get_texture() const { return m_texture; }
        std::array<glm::vec2, 4> const& texture_coords() const { return m_texture_coords; }

        // Calculates the portion of texture from which create the subtexture.
        // texture - reference texture
        // coords - the indexes on x and y axis of the sprite.
        // cell_size - the size in pixels of all the sprites in the spritesheet.
        // sprite_size - the number of cells the specific sprite takes on the spritesheet. The defaul value is {1,1}.
        static ref<sub_texture_2d> 
            create_from_coords(
                ref<texture_2d> texture, 
                glm::vec2 const &coords, 
                glm::vec2 const &cell_size, 
                glm::vec2 const &sprite_size = { 1.f ,1.f });
    private:
        ref<texture_2d> m_texture;
        std::array<glm::vec2, 4> m_texture_coords{};
    };
}