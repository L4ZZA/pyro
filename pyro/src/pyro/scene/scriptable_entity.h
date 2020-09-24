#pragma once
#include "pyro/scene/entity.h"

namespace pyro
{
    class scriptable_entity
    {
    public:
        virtual ~scriptable_entity() = default;

        template<typename T>
        T &get_component()
        {
            return m_entity.get_component<T>();
        }

    protected:
        virtual void on_create(){}
        virtual void on_destroy() {}
        virtual void on_update(timestep const &ts){}

    private:
        entity m_entity;
        friend class scene;
    };
}