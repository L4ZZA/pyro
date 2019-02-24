#include "pyro_pch.h"
#include "layers_stack.h"

pyro::layers_stack::layers_stack()
{
}

pyro::layers_stack::~layers_stack()
{
    for (auto* layer : m_layers)
        delete layer;
}

void pyro::layers_stack::push_layer(layer* p_layer)
{
     m_layers.emplace(m_layers.begin() + m_layers_insert_index, p_layer);
     m_layers_insert_index++;
}

void pyro::layers_stack::push_overlay(layer* p_overlay)
{
    m_layers.emplace_back(p_overlay);
}

void pyro::layers_stack::pop_layer(layer* p_layer)
{
    auto it = std::find(m_layers.begin(), m_layers.end(), p_layer);
    if(it != m_layers.end())
    {
        m_layers.erase(it);
        --m_layers_insert_index;
    }
}

void pyro::layers_stack::pop_overlay(layer* p_overlay)
{
    auto it = std::find(m_layers.begin(), m_layers.end(), p_overlay);
    if (it != m_layers.end())
        m_layers.erase(it);
}
