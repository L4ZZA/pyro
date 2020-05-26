#pragma once
#include "renderer/scene.h"
#include <vector>

class scene_manager
{
public:
	void add_scene(pyro::ref<scene> scene);
	// initializes the current scene.
	void init_first_scene();
	void next_scene();
	void previous_scene();
	void go_to(int scene_index);

	void on_update(pyro::timestep const &ts);
	void on_render() const;
	void on_event(pyro::event &e);
	void on_imgui_render();
private:
	std::vector<pyro::ref<scene>> m_scenes;
	int m_scene_index = -1;
};
