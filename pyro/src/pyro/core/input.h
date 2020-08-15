#pragma once
#include "pyro/core/core.h"

namespace pyro
{
    /// Base class for input polling.
    /// input polling allows to retrieve input states at any time.
    class PYRO_API input
    {
    public:
        /// Returns if the specified key is being pressed.
        static bool key_pressed(int p_key_code);

        /// Returns if the specified mouse button is being pressed.
        static bool mouse_button_pressed(int p_button);

        /// Returns the mouse position as a pair of float values.
        static std::pair<float, float> mouse_position();
        /// Returns the x coordinate of the mouse position.
        static float mouse_x();
        /// Returns the y coordinate of the mouse position.
        static float mouse_y();
    };
}