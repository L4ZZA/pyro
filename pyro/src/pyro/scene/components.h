#pragma once
#include <glm/glm.hpp>
#include <string>
#include "scene_camera.h"

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

}