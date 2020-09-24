#include "scene_hierarchy_panel.h"
#include "pyro/scene/components.h"
#include <imgui/imgui.h>

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

		ImGui::End();
	}

	void scene_hierarchy_panel::draw_entity_node(entity &e)
	{
		auto &tag = e.get_component<tag_component>().tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == e) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void *>((uint64_t)(uint32_t)e), flags, tag.c_str());
		if(ImGui::IsItemClicked())
		{
			m_SelectionContext = e;
		}

		if(opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(9817239), flags, tag.c_str());
			if(opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}

	void scene_hierarchy_panel::context(ref<scene> scene_context)
	{
		m_context = scene_context;
	}
}
