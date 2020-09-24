#pragma once
#include "pyro/core/core.h"
#include "pyro/core/logger.h"
#include "pyro/scene/scene.h"
#include "pyro/scene/entity.h"

namespace pyro
{
	class scene_hierarchy_panel
	{
	public:
		scene_hierarchy_panel() = default;
		scene_hierarchy_panel(ref<scene> scene_context);
		~scene_hierarchy_panel();
		void on_imgui_render();
		void context(ref<scene> scene_context);

	private:
		void draw_entity_node(entity e);
		void draw_components(entity e);

	private:
		ref<scene> m_context;
		entity m_selection_context;
	};
}