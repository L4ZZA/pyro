#pragma once
#include "pyro/scene/scene_camera.h"
#include "pyro/scene/scriptable_entity.h"
#include <glm/glm.hpp>
#include <string>

namespace pyro
{
	struct tag_component
	{
		inline static char *type_name = "tag_component";

		std::string tag;

		tag_component() = default;
		tag_component(const tag_component &) = default;
		tag_component(const std::string &t)
			: tag(t)
		{
		}
	};

	struct transform_component
	{
		inline static char *type_name = "transform_component";
		glm::mat4 transform{ 1.0f };

		transform_component() = default;
		transform_component(const transform_component &) = default;
		transform_component(const glm::mat4 &t)
			: transform(t)
		{
		}

		operator glm::mat4 &() { return transform; }
		operator const glm::mat4 &() const { return transform; }
	};

	struct sprite_renderer_component
	{
		inline static char *type_name = "sprite_renderer_component";
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

		sprite_renderer_component() = default;
		sprite_renderer_component(const sprite_renderer_component &) = default;
		sprite_renderer_component(const glm::vec4 &c)
			: color(c)
		{}
	};


	struct camera_component
	{
		inline static char *type_name = "camera_component";

		scene_camera camera;
		bool primary = true; // TODO: think about moving to Scene
		bool fixed_aspect_ratio = false;

		camera_component() = default;
		camera_component(const camera_component &) = default;
	};

	struct native_script_component
	{
		inline static char *type_name = "native_script_component";

		scriptable_entity *instance = nullptr;

		std::function<void()> InstantiateFunction;
		std::function<void()> DestroyInstanceFunction;

		std::function<void(scriptable_entity *)> OnCreateFunction;
		std::function<void(scriptable_entity *)> OnDestroyFunction;
		std::function<void(scriptable_entity *, timestep const&)> OnUpdateFunction;

		template<typename T>
		void bind()
		{
			InstantiateFunction = [&]() { instance = new T(); };
			DestroyInstanceFunction = [&]() { delete (T *)instance; instance = nullptr; };

			OnCreateFunction = [](scriptable_entity *instance) { ((T *)instance)->on_create(); };
			OnDestroyFunction = [](scriptable_entity *instance) { ((T *)instance)->on_destroy(); };
			OnUpdateFunction = [](scriptable_entity *instance, timestep const &ts) { ((T *)instance)->on_update(ts); };
		}
	};

}