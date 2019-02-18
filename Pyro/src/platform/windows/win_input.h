#pragma once

#include "pyro/input.h"

namespace pyro
{
    class win_input : public input
    {
    public:
        bool key_pressed_impl(int p_key_code) const override;
        bool mouse_button_pressed_impl(int p_button) const override;

    protected:
        std::pair<float, float> mouse_position_impl() const override;
        float mouse_x_impl() const override;
        float mouse_y_impl() const override;
    };
}
