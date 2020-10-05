#include "pyro_pch.h"
#include "layer.h"

pyro::layer::layer(
    uint32_t width, uint32_t height, 
    std::string const &name /*= "Layer"*/)
    : m_layer_width(width)
    , m_layer_height(height)
    , m_debug_name(name)
{
}

pyro::layer::~layer()
{
}
