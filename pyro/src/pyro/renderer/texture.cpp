#include "pyro_pch.h"
#include "pyro/renderer/texture.h"

#include "pyro/renderer/renderer.h"
#include "platform/opengl/gl_texture.h"


pyro::ref<pyro::texture_2d>
pyro::texture_2d::create(
    uint32_t width,
    uint32_t height,
    texture_parameters const &params /*= texture_parameters()*/)
{
    switch(renderer::api())
    {
        case e_renderer_api::none: PYRO_CORE_ASSERT(false, "[texture_2d] e_renderer_api::none currently not supported!"); return nullptr;
        case e_renderer_api::opengl: return make_ref<gl_texture_2d>(width, height, params);
    }

    PYRO_CORE_ASSERT(false, "[texture_2d] Unknown renderer api!");
    return nullptr;
}

std::shared_ptr<pyro::texture_2d>
pyro::texture_2d::create_from_file(
    std::string const &path,
    texture_parameters const &params /*= texture_parameters()*/)
{
    switch(renderer::api())
    {
        case e_renderer_api::none: PYRO_CORE_ASSERT(false, "[texture_2d] e_renderer_api::none currently not supported!"); return nullptr;
        case e_renderer_api::opengl: return make_ref<gl_texture_2d>(path, params);
    }

    PYRO_CORE_ASSERT(false, "[texture_2d] Unknown renderer api!");
    return nullptr;
}

//------------------------------------------------------------------------------------------------

void 
pyro::texture_library::add(const ref<texture_2d> &shader)
{
    std::string path = shader->path();
    if(!exists(path))
    {
        m_textures[path] = shader;
    }
    else
    {
        PYRO_CORE_WARN("[texture_library] {} shader already exists!", path);
    }
}

pyro::ref<pyro::texture_2d> 
pyro::texture_library::load(std::string const &filepath)
{
    ref<texture_2d> texture = get(filepath);
    if(!texture)
    {
        texture = texture_2d::create_from_file(filepath);
        add(texture);
    }
    return texture;
}

pyro::ref<pyro::texture_2d> 
pyro::texture_library::get(std::string const &name)
{
    if(!exists(name))
    {
        return nullptr;
        //PYRO_CORE_DEBUG("[texture_library] shader not found!");
    }
    return m_textures[name];
}

bool 
pyro::texture_library::exists(std::string const &name)
{
    return m_textures.find(name) != m_textures.end();
}
