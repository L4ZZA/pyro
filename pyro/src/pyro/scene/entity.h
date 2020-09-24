#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace pyro
{

    class entity
    {
    public:
        entity() = default;
        entity(entt::entity handle, scene *scene);
        entity(const entity &other) = default;

        template<typename T, typename... Args>
        T &add_component(Args &&... args)
        {
            T obj;
            std::string message("entity::add_component - entity already has a ");
            message += obj.type_name;
            message += " component!";
            PYRO_CORE_ASSERT(!has_component<T>(), message);
            return m_scene->m_registry.emplace<T>(m_entity_handle, std::forward<Args>(args)...);
        }

        template<typename T>
        T &get_component()
        {
            T obj;
            std::string message("entity::get_component - entity does not have a ");
            message += obj.type_name;
            message += " component!";
            PYRO_CORE_ASSERT(has_component<T>(), message);
            return m_scene->m_registry.get<T>(m_entity_handle);
        }

        template<typename T>
        bool has_component()
        {
            return m_scene->m_registry.has<T>(m_entity_handle);
        }

        template<typename T>
        void remove_component()
        {
            T obj;
            std::string message("entity::remove_component - entity does not have a ");
            message += obj.type_name;
            message += " component!";
            PYRO_CORE_ASSERT(has_component<T>(), message);
            m_scene->m_registry.remove<T>(m_entity_handle);
        }

        operator bool() const { return m_entity_handle != entt::null; }
        operator uint32_t() const { return static_cast<uint32_t>(m_entity_handle); }

        bool operator==(const entity &other) const
        {
            return m_entity_handle == other.m_entity_handle && m_scene == other.m_scene;
        }

        bool operator!=(const entity &other) const
        {
            return !(*this == other);
        }

    private:
        entt::entity m_entity_handle{ entt::null };
        scene *m_scene = nullptr;
    };
}