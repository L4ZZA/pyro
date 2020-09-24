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

		if(e.has_component<camera_component>())
		{
			if(ImGui::TreeNodeEx(
				reinterpret_cast<void *>(typeid(camera_component).hash_code()),
				ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto &camera_comp = e.get_component<camera_component>();
				auto &camera = camera_comp.camera;

				const char *projection_type_strings[]{ "Perspective", "Otrhographic" };
				const char *current_projection_type_string =
					projection_type_strings[static_cast<int>(camera.projection_type())];

				if(ImGui::BeginCombo("Projection", current_projection_type_string))
				{
					for(int i = 0; i < 2; i++)
					{
						bool is_selected = 
							current_projection_type_string == projection_type_strings[i];
						if(ImGui::Selectable(projection_type_strings[i], is_selected))
						{
							current_projection_type_string = projection_type_strings[i];
							camera.projection_type(static_cast<scene_camera::e_projection_type>(i));
						}

						if(is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::Checkbox("Primary", &camera_comp.primary);

				if(camera.projection_type() == scene_camera::e_projection_type::perspective)
				{
					float perspective_vertical_fov = glm::degrees(camera.perspective_vertical_fov());
					if(ImGui::DragFloat("Size", &perspective_vertical_fov))
					{
						camera.perspective_vertical_fov(glm::radians(perspective_vertical_fov));
					}

					float perspective_near = camera.perspective_near();
					if(ImGui::DragFloat("Near", &perspective_near))
					{
						camera.perspective_near(perspective_near);
					}

					float perspective_far = camera.perspective_far();
					if(ImGui::DragFloat("Far", &perspective_far))
					{
						camera.perspective_far(perspective_far);
					}
				}

				if(camera.projection_type() == scene_camera::e_projection_type::orthographic)
				{
					ImGui::Checkbox("Fixed Aspect ratio", &camera_comp.fixed_aspect_ratio);

					float ortho_size = camera.orthographic_size();
					if(ImGui::DragFloat("Size", &ortho_size))
					{
						camera.orthographic_size(ortho_size);
					}

					float ortho_near = camera.orthographic_near();
					if(ImGui::DragFloat("Near", &ortho_near))
					{
						camera.orthographic_near(ortho_near);
					}

					float ortho_far = camera.orthographic_far();
					if(ImGui::DragFloat("Far", &ortho_far))
					{
						camera.orthographic_far(ortho_far);
					}
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
