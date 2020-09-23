#pragma once
#include <pyro.h>
#include "renderer/scene.h"
#include "utils/noise.h"
#include "utils/perlin_noise.h"

class base_noise_scene : public scene
{
public:
	base_noise_scene(pyro::ref<pyro::camera_base> const &camera);
	virtual ~base_noise_scene() = default;
	virtual void play() = 0;
	virtual void stop_playing() = 0;
	virtual bool is_playing() = 0;
protected:
	virtual void on_seed_changed() = 0;
};

inline 
base_noise_scene::base_noise_scene(pyro::ref<pyro::camera_base> const &camera) 
	: scene(camera) 
{
}