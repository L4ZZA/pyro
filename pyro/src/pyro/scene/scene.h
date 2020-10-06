#pragma once
#include <entt.hpp>
#include <glm/glm.hpp>
#include "pyro/core/timestep.h"
#include "pyro/events/event.h"

namespace pyro
{
    class entity;

    class scene
    {
    public:
        scene();
        ~scene();

        entity create_entity(
            std::string const &name = std::string(), 
            glm::vec3 const &position = { 0.f, 0.f, 0.f });
        void on_update(timestep const &ts);
        void on_render();
        void on_event(pyro::event &e);
        void on_viewport_resize(uint32_t width, uint32_t height);

        uint32_t width () const {return m_viewport_width; }
        uint32_t height() const {return m_viewport_width; }

    private:
        entt::registry m_registry;
        uint32_t m_viewport_width  = 0;
        uint32_t m_viewport_height = 0;
        friend class entity;
        friend class scene_hierarchy_panel;
    };
}