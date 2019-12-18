#include "pyro_pch.h"
#include "texture.h"
#include "pyro_pch.h" 
#include "texture.h" 
 
pyro::texture_parameters::texture_parameters() 
    :format(e_texture_format::rgba), 
    filter(e_texture_filter::linear), 
    wrap(e_texture_wrap::clamp_to_edge) 
{ 
} 
 
pyro::texture_parameters::texture_parameters( 
    e_texture_filter filter) 
    :format(e_texture_format::rgba), filter(filter), wrap(e_texture_wrap::clamp_to_edge) 
{ 
} 
 
pyro::texture_parameters::texture_parameters( 
    e_texture_filter filter, 
    e_texture_wrap wrap) 
    : format(e_texture_format::rgba), filter(filter), wrap(wrap) 
{ 
} 
 
pyro::texture_parameters::texture_parameters( 
    e_texture_format format, 
    e_texture_filter filter, 
    e_texture_wrap wrap) 
    : format(format), filter(filter), wrap(wrap) 
{ 
}

//----------------------------------------------------------------------------- 

pyro::e_texture_wrap pyro::texture::s_wrap_mode = pyro::e_texture_wrap::clamp_to_edge;
pyro::e_texture_filter pyro::texture::s_filter_mode = pyro::e_texture_filter::linear;

void pyro::texture::wrap(e_texture_wrap mode)
{
    s_wrap_mode = mode; 
}