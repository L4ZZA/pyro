#include "scene_manager.h"

void scene_manager::add_scene(pyro::ref<scene> scene)
{
    m_scenes.push_back(scene);
}

void scene_manager::init_first_scene()
{
    m_scene_index = 0;
    m_scenes[m_scene_index]->init();
}

void scene_manager::next_scene()
{
    if(m_scene_index < m_scenes.size() - 1)
    {
        m_scenes[m_scene_index]->deinit();
        m_scene_index++;
        m_scenes[m_scene_index]->init();
    }
    else
    {
        PYRO_ERROR("scene_manager::next_scene - no more scenes.");
    }
}

void scene_manager::previous_scene()
{
    if(m_scene_index > 0)
    {
        m_scenes[m_scene_index]->deinit();
        m_scene_index--;
        m_scenes[m_scene_index]->init();
    }
    else
    {
        PYRO_ERROR("scene_manager::previous_scene - no previous scenes.");
    }
}

void scene_manager::go_to(int scene_index)
{
    if(scene_index > -1 || scene_index > m_scenes.size() - 1)
    {
        m_scenes[m_scene_index]->deinit();
        m_scene_index = scene_index;
        m_scenes[m_scene_index]->init();
    }
    else
    {
        PYRO_ERROR("scene_manager::go_to - scene {} does not exist.", scene_index);
    }
}

void scene_manager::on_update(pyro::timestep const &ts)
{
    if(m_scenes.empty())
        PYRO_ERROR("scene_manager::go_to - NO SCENES HAVE BEEN ADDED YET.");

    // if 
    if(m_scene_index == -1)
        next_scene();

    m_scenes[m_scene_index]->on_update(ts);
}
void scene_manager::on_render() const
{
    m_scenes[m_scene_index]->on_render();
}

void scene_manager::on_event(pyro::event &e)
{
    m_scenes[m_scene_index]->on_event(e);
}

void scene_manager::on_imgui_render()
{
    m_scenes[m_scene_index]->on_imgui_render();
}
