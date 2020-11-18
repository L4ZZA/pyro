#include "pyro_pch.h"
#include "scene_serializer.h"
#include "entity.h"
#include "components.h"

#include <fstream>
#include <filesystem>
#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace pyro
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static void serialize_entity(YAML::Emitter& out, pyro::entity ent)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "entity" << YAML::Value << "12837192831273"; // TODO: Entity ID goes here

		if (ent.has_component<tag_component>())
		{
			out << YAML::Key << "tag_component";
			out << YAML::BeginMap; // tag_component

			auto& tag = ent.get_component<tag_component>().tag;
			out << YAML::Key << "tag" << YAML::Value << tag;

			out << YAML::EndMap; // tag_component
		}

		if (ent.has_component<transform_component>())
		{
			out << YAML::Key << "transform_component";
			out << YAML::BeginMap; // transform_component

			auto& tc = ent.get_component<transform_component>();
			out << YAML::Key << "translation" << YAML::Value << tc.translation;
			out << YAML::Key << "rotation" << YAML::Value << tc.rotation;
			out << YAML::Key << "scale" << YAML::Value << tc.scale;

			out << YAML::EndMap; // transform_component
		}

		if (ent.has_component<camera_component>())
		{
			out << YAML::Key << "camera_component";
			out << YAML::BeginMap; // camera_component

			auto& cameraComponent = ent.get_component<camera_component>();
			auto& camera = cameraComponent.camera;

			out << YAML::Key << "camera" << YAML::Value;
			out << YAML::BeginMap; // camera
			out << YAML::Key << "projection_type" << YAML::Value << (int)camera.projection_type();
			out << YAML::Key << "perspective_vertical_fov" << YAML::Value << camera.perspective_vertical_fov();
			out << YAML::Key << "perspective_near" << YAML::Value << camera.perspective_near();
			out << YAML::Key << "perspective_far" << YAML::Value << camera.perspective_far();
			out << YAML::Key << "orthographic_size" << YAML::Value << camera.orthographic_size();
			out << YAML::Key << "orthographic_near" << YAML::Value << camera.orthographic_near();
			out << YAML::Key << "orthographic_far" << YAML::Value << camera.orthographic_far();
			out << YAML::EndMap; // camera

			out << YAML::Key << "primary" << YAML::Value << cameraComponent.primary;
			out << YAML::Key << "fixed_aspect_ratio" << YAML::Value << cameraComponent.fixed_aspect_ratio;

			out << YAML::EndMap; // camera_component
		}

		if (ent.has_component<sprite_renderer_component>())
		{
			out << YAML::Key << "sprite_renderer_component";
			out << YAML::BeginMap; // sprite_renderer_component

			auto& spriteRendererComponent = ent.get_component<sprite_renderer_component>();
			auto& sub_texture = ent.get_component<sprite_renderer_component>().sub_texture;
			auto& texture_coords = ent.get_component<sprite_renderer_component>().texture_coords;

			out << YAML::Key << "color" << YAML::Value << spriteRendererComponent.color;
			
			out << YAML::Key << "sub_texture" << YAML::Value;
			out << YAML::BeginMap; // sub_texture
			out << YAML::Key << "path" << YAML::Value << sub_texture->get_texture()->path();
			out << YAML::Key << "min" << YAML::Value << texture_coords[0];
			out << YAML::Key << "max" << YAML::Value << texture_coords[2];
			out << YAML::EndMap; // sub_texture

			out << YAML::EndMap; // sprite_renderer_component
		}

		out << YAML::EndMap; // Entity
	}
}

pyro::scene_serializer::scene_serializer(const ref<scene> &scene)
	: m_scene(scene)
{
}

void pyro::scene_serializer::serialize(const std::string &filepath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "scene" << YAML::Value << "Untitled";
	out << YAML::Key << "entities" << YAML::Value << YAML::BeginSeq;
	m_scene->m_registry.each([&](auto entityID)
	{
		entity ent = { entityID, m_scene.get() };
		if (!ent)
			return;

		serialize_entity(out, ent);
	});
	out << YAML::EndSeq;
	out << YAML::EndMap;

	auto path = std::filesystem::path(filepath).remove_filename();
	std::filesystem::create_directories(path);
	std::ofstream fout(filepath);
	if(fout.is_open())
	{
		fout << out.c_str();
	}
	else
	{
		PYRO_CORE_ASSERT(false, "pyro::scene_serializer::serialize failed to write to file");
	}
}

void pyro::scene_serializer::serialize_runtime(const std::string &filepath)
{
	// Not implemented
	PYRO_CORE_ASSERT(false, "pyro::scene_serializer::serialize_runtime not implemented");
}

bool pyro::scene_serializer::deserialize(const std::string &filepath)
{
	YAML::Node data = YAML::LoadFile(filepath);
	if (!data["scene"])
		return false;

	std::string sceneName = data["scene"].as<std::string>();
	PYRO_CORE_TRACE("Deserializing scene '{0}'", sceneName);

	auto entities = data["entities"];
	if (entities)
	{
		for (auto ent : entities)
		{
			uint64_t uuid = ent["entity"].as<uint64_t>(); // TODO

			std::string name;
			auto tagComponent = ent["tag_component"];
			if (tagComponent)
				name = tagComponent["tag"].as<std::string>();

			PYRO_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

			entity deserialized_entity = m_scene->create_entity(name);

			auto transformComponent = ent["transform_component"];
			if (transformComponent)
			{
				// Entities always have transforms
				auto& tc = deserialized_entity.get_component<transform_component>();
				tc.translation = transformComponent["translation"].as<glm::vec3>();
				tc.rotation = transformComponent["rotation"].as<glm::vec3>();
				tc.scale = transformComponent["scale"].as<glm::vec3>();
			}

			auto cameraComponent = ent["camera_component"];
			if (cameraComponent)
			{
				auto& cc = deserialized_entity.add_component<camera_component>();

				auto& cameraProps = cameraComponent["camera"];
				cc.camera.projection_type((scene_camera::e_projection_type)cameraProps["projection_type"].as<int>());

				cc.camera.perspective_vertical_fov(cameraProps["perspective_vertical_fov"].as<float>());
				cc.camera.perspective_near(cameraProps["perspective_near"].as<float>());
				cc.camera.perspective_far(cameraProps["perspective_far"].as<float>());

				cc.camera.orthographic_size(cameraProps["orthographic_size"].as<float>());
				cc.camera.orthographic_near(cameraProps["orthographic_near"].as<float>());
				cc.camera.orthographic_far(cameraProps["orthographic_far"].as<float>());

				cc.primary = cameraComponent["primary"].as<bool>();
				cc.fixed_aspect_ratio = cameraComponent["fixed_aspect_ratio"].as<bool>();
			}

			auto spriteRendererComponent = ent["sprite_renderer_component"];
			if (spriteRendererComponent)
			{
				auto color = spriteRendererComponent["color"].as<glm::vec4>();
				auto sub_texture = spriteRendererComponent["sub_texture"];
				if(sub_texture)
				{
					auto path = sub_texture["path"].as<std::string>();
					auto min = sub_texture["min"].as<glm::vec2>();
					auto max = sub_texture["max"].as<glm::vec2>();
					deserialized_entity.add_component<sprite_renderer_component>(path, min, max, color);
				}
			}
		}
	}

	return true;
}

bool pyro::scene_serializer::deserialize_runtime(const std::string &filepath)
{
	// Not implemented
	PYRO_CORE_ASSERT(false, "pyro::scene_serializer::deserialize_runtime not implemented");
	return false;
}
