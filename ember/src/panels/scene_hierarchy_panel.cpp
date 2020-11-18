#include "scene_hierarchy_panel.h"
#include "pyro/scene/components.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace pyro
{
	
	static void draw_vec3_control(const std::string& label, glm::vec3& values, 
		float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO &io = ImGui::GetIO();
		auto bold_font = io.Fonts->Fonts[1];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(bold_font);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(bold_font);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(bold_font);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template <typename T, typename UI_FUNCTION>
	static void draw_component(std::string const &name, entity e, UI_FUNCTION uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = 
			ImGuiTreeNodeFlags_DefaultOpen | 
			ImGuiTreeNodeFlags_Framed | 
			ImGuiTreeNodeFlags_SpanAvailWidth | 
			ImGuiTreeNodeFlags_AllowItemOverlap | 
			ImGuiTreeNodeFlags_FramePadding;

		if(e.has_component<T>())
		{
			auto &comp = e.get_component<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar(
			);
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if(ImGui::BeginPopup("ComponentSettings"))
			{
				if(ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if(open)
			{
				uiFunction(comp);
				ImGui::TreePop();
			}

			if(removeComponent)
				e.remove_component<T>();
		}
	}

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
		
		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_context->create_entity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if(m_selection_context)
		{
			draw_components(m_selection_context);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_selection_context.add_component<camera_component>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_selection_context.add_component<sprite_renderer_component>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

		}
		ImGui::End();
	}

	void scene_hierarchy_panel::draw_entity_node(entity e)
	{
		auto &tag = e.get_component<tag_component>().tag;

		ImGuiTreeNodeFlags flags = ((m_selection_context == e) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void *>((uint64_t)(uint32_t)e), flags, tag.c_str());
		if(ImGui::IsItemClicked())
		{
			m_selection_context = e;
		}
		
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if(opened)
		{
			// TODO: remove this duplicated tag text when parenting is implemented.
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(9817239), flags, tag.c_str());
			if(opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
		
		if (entityDeleted)
		{
			m_context->destroy_entity(e);
			if (m_selection_context == e)
				m_selection_context = {};
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

		draw_component<transform_component>("Transform", e, [](auto &tc) 
		{
			draw_vec3_control("Translation", tc.translation);
			glm::vec3 rotation = glm::degrees(tc.rotation);
			draw_vec3_control("Rotation", rotation);
			tc.rotation = glm::radians(rotation);
			draw_vec3_control("Scale", tc.scale, 1.0f);
		});
		
		draw_component<sprite_renderer_component>("Sprite Renderer", e, [](auto &comp)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(comp.color));
		});
		
		draw_component<camera_component>("Camera", e, [](auto &comp)
		{
				
			auto &camera = comp.camera;

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

			ImGui::Checkbox("Primary", &comp.primary);

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
				ImGui::Checkbox("Fixed Aspect ratio", &comp.fixed_aspect_ratio);

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
		});

	}

	void scene_hierarchy_panel::context(ref<scene> scene_context)
	{
		m_context = scene_context;
		m_selection_context = {};
	}
}
