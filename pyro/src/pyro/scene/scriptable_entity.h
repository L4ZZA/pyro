#pragma once
#include "pyro/scene/entity.h"

namespace pyro
{
    class scriptable_entity
    {
    public:

        template<typename T>
        T &get_component()
        {
            return m_entity.get_component<T>();
        }

    private:
        entity m_entity;
        friend class scene;
    };
}