﻿#pragma once
#include "pyro/renderer/texture.h"

namespace pyro
{
    class PYRO_API texture_2d : public texture
    {
    public:
        static ref<texture_2d> create(
            uint32_t width, 
            uint32_t height, 
            texture_parameters const &params = texture_parameters());
        static ref<texture_2d> create_from_file(
            std::string const &path, 
            texture_parameters const &params = texture_parameters());
    };
}
