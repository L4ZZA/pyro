#pragma once
#include "pyro.h"
#include "utils/noise.h"
#include "utils/perlin_noise.h"

class base_noise_scene : public pyro::scene
{
public:
	base_noise_scene(pyro::ref<pyro::camera> const &camera);
	virtual ~base_noise_scene() = default;
protected:
	virtual void on_seed_changed() = 0;
};

inline 
base_noise_scene::base_noise_scene(pyro::ref<pyro::camera> const &camera) 
	: scene(camera) 
{
}