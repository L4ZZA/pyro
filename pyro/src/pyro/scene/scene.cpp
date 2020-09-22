#include "pyro_pch.h"
#include "scene.h"
#include "entity.h"

#include "components.h"
#include "pyro/renderer/renderer_2d.h"


pyro::scene::scene() {}
pyro::scene::~scene() {}

pyro::entity pyro::scene::create_entity(std::string const &name /*= ""*/)
{
	entity entity = { m_registry.create(), this };
	entity.add_component<transform_component>();
	auto &tag = entity.add_component<tag_component>();
	tag.tag = name.empty() ? "entity" : name;
	return entity;
}

void pyro::scene::on_update(pyro::timestep const &ts)
{
}

void pyro::scene::on_render()
{
	// Render sprites
	editor_camera *main_camera = nullptr;
	glm::mat4 *camera_transform = nullptr;
	{
		auto group = m_registry.view<transform_component, camera_component>();
		for(auto entity : group)
		{
			auto [transformComp, cameraComp] = group.get<transform_component, camera_component>(entity);

			if(cameraComp.primary)
			{
				main_camera = &cameraComp.camera;
				camera_transform = &transformComp.transform;
				break;
			}
		}
	}
	
	if(main_camera)
	{
		renderer_2d::begin_scene(main_camera->projection_matrix(), *camera_transform);

		auto group = m_registry.group<transform_component>(entt::get<sprite_renderer_component>);
		for(auto entity : group)
		{
			auto &[transform, sprite] = group.get<transform_component, sprite_renderer_component>(entity);

			renderer_2d::draw_quad(transform, sprite.color);
		}

		renderer_2d::end_scene();
	}
}

void pyro::scene::on_viewport_resize(uint32_t width, uint32_t height)
{ 
}
