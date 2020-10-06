#include "pyro_pch.h"
#include "scene.h"
#include "entity.h"

#include "components.h"
#include "pyro/renderer/renderer_2d.h"


pyro::scene::scene() {}
pyro::scene::~scene() {}

pyro::entity pyro::scene::create_entity(
	std::string const &name /*= ""*/, 
    glm::vec3 const &position /*= { 0.f, 0.f, 0.f }*/)
{
	entity entity = { m_registry.create(), this };
	entity.add_component<transform_component>(position);
	auto &tag = entity.add_component<tag_component>();
	tag.tag = name.empty() ? "entity" : name;
	return entity;
}

void pyro::scene::on_update(pyro::timestep const &ts)
{
	// Update scripts
	{
		m_registry.view<native_script_component>().each([=](auto entity, native_script_component &nsc)
			{
				// TODO:: move to on_scene_play 
				if(!nsc.instance)
				{
					nsc.instance = nsc.instantiate_script_func();
					nsc.instance->m_entity = pyro::entity{ entity, this };
					nsc.instance->on_create();
				}

				nsc.instance->on_update(ts);
			});
	}
}

void pyro::scene::on_render()
{
	// Render sprites
	camera *main_camera = nullptr;
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
			auto [transform, sprite] = group.get<transform_component, sprite_renderer_component>(entity);

			quad_properties props;
			props.transform = transform;
			props.texture = sprite.texture;
			props.texture_coords = sprite.texture_coords;
			props.color = sprite.color;
			renderer_2d::draw_quad(props);
		}

		renderer_2d::end_scene();
	}
}

void pyro::scene::on_event(pyro::event &e)
{
	m_registry.view<native_script_component>().each([&](native_script_component &nsc)
		{
			if(nsc.instance)
			{
				nsc.instance->on_event(e);
			}

		});
}

void pyro::scene::on_viewport_resize(uint32_t width, uint32_t height)
{
	m_viewport_width  = width;
	m_viewport_height = height;

	// Resize our non-FixedAspectRatio cameras
	auto view = m_registry.view<camera_component>();
	for(auto entity : view)
	{
		auto &cameraComponent = view.get<camera_component>(entity);
		if(!cameraComponent.fixed_aspect_ratio)
			cameraComponent.camera.viewport_size(width, height);
	}
}
