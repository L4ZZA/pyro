#pragma once
#include "entt.hpp"
#include "pyro/core/timestep.h"

namespace pyro
{
    class entity;

    class scene
    {
    public:
        scene();
        ~scene();

        entity create_entity(std::string const &name = std::string());
        void on_update(timestep const &ts);
        void on_render();
        void on_viewport_resize(uint32_t width, uint32_t height);

    private:
        entt::registry m_registry;
        uint32_t m_width  = 0;
        uint32_t m_height = 0;
        friend class entity;
    };
}