#include "scene_hierarchy_panel.h"
#include "pyro/scene/components.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace pyro
{

	scene_hierarchy_panel::scene_hierarchy_panel(ref<scene> scene_context)
	{
		context(scene_context);
	}

	scene_hierarchy_panel::~scene_hierarchy_panel()
	{
	}

	void scene_hierarchy_panel::on_imgui_render()
	{
		ImGui::Begin("Scene hierarchy panel");

		m_context->m_registry.each([&](auto entity_id)
			{
				entity ent{ entity_id, m_context.get() };
				draw_entity_node(ent);
			});

		// ability to deselect node 
		if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_selection_context = {};

		ImGui::End();

		ImGui::Begin("Properties");
		if(m_selection_context)
		{
			draw_components(m_selection_context);
		}
		ImGui::End();
	}

	void scene_hierarchy_panel::draw_entity_node(entity e)
	{
		auto &tag = e.get_component<tag_component>().tag;

		ImGuiTreeNodeFlags flags = ((m_selection_context == e) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void *>((uint64_t)(uint32_t)e), flags, tag.c_str());
		if(ImGui::IsItemClicked())
		{
			m_selection_context = e;
		}

		if(opened)
		{
			// TODO: remove this duplicated tag text
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(9817239), flags, tag.c_str());
			if(opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}

	void scene_hierarchy_panel::draw_components(entity e)
	{
		if(e.has_component<tag_component>())
		{
			auto &tag = e.get_component<tag_component>().tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if(ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if(e.has_component<transform_component>())
		{
			if(ImGui::TreeNodeEx(
				reinterpret_cast<void *>(typeid(transform_component).hash_code()), 
				ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto &transform = e.get_component<transform_component>().transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
				static glm::vec3 scale{ transform[0][0], transform[1][1], transform[2][2] };
				if(ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f))
				{
					transform = glm::scale(glm::mat4{1.f}, scale);
				}

				ImGui::TreePop();
			}
		}
	}
	void scene_hierarchy_panel::context(ref<scene> scene_context)
	{
		m_context = scene_context;
	}
}
