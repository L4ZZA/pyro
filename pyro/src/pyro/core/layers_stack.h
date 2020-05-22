#pragma once

#include "pyro/core/core.h"
#include "layer.h"

namespace pyro
{
    /// Wrapper around a vector of layers.
    /// It's not an actual stack as we need to push layers in the middle of the stack.
    class PYRO_API layers_stack
    {
    public:
        layers_stack() = default;
        ~layers_stack();

        /// Adds the layer to the first half of the layers_stack.
        void push_layer(ref<layer> const &layer);
        /// Adds the overlay to the second half of the layers_stack.
        /// Every new overlay will be added to the very end of the 
        void push_overlay(ref<layer> const &overlay);
        /// Removes the layer from the layers_stack. Layer won't be deleted.
        /// Returns true if the layer has been removed, false otherwise.
        bool pop_layer(ref<layer> const &layer);
        /// Removes the overlay from the layers_stack. Overlay won't be deleted.
        /// Returns true if the layer has been removed, false otherwise.
        bool pop_overlay(ref<layer> const &overlay);

        // To iterate through a range-based for-loop [i.e. for( auto elem : elements)]
        /// Implicit implementation of .begin()
        std::vector<ref<layer>>::iterator begin() { return m_layers.begin(); }
        /// Implicit implementation of .end()
        std::vector<ref<layer>>::iterator end() { return m_layers.end(); }

    private:
        std::vector<ref<layer>> m_layers;
        uint32_t m_layers_insert_index{0};
    };
}
