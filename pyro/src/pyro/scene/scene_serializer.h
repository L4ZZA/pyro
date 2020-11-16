#pragma once
#include "Scene.h"

namespace pyro
{
	class scene_serializer
	{
	public:
		scene_serializer(const ref<scene>& scene);

		void serialize(const std::string& filepath);
		void serialize_runtime(const std::string& filepath);

		bool deserialize(const std::string& filepath);
		bool deserialize_runtime(const std::string& filepath);
	private:
		ref<scene> m_scene;
	};
}