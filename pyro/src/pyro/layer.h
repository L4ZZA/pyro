#pragma once

#include "pyro/core.h"
#include "pyro/events/event.h"
#include "core/timestep.h"

namespace pyro
{
	/// \brief Interface/base-class from which implement the layers in your applicaiton.
	class PYRO_API layer
	{
	public:
		layer(const std::string &p_name = "Layer");
		virtual ~layer();

		/// \brief Runs when a layer is pushed onto the layer stack
		virtual void on_attach() {}
		/// \brief Runs when a layer is popped from the layer stack
		virtual void on_detach() {}
		/// \brief Runs when application calls the on_update method.
		/// Should happen every frame.
		virtual void on_update(timestep timestep) {}
		/// \brief Render method specific to imgui.
		virtual void on_imgui_render() {}
		/// \brief Runs whenever an event gets sent to the layer.
		virtual void on_event(event &p_event) {}

		/// \brief Gives you the name of the layer [for debug]
		const std::string& name() const { return m_debug_name; }
		bool is_imgui() const { return m_imgui; }

	protected:
		std::string m_debug_name{};

		/// \brief Flag to distinguish between imgui and pyro rendering calls.
		bool		m_imgui{ false };

		//TODO: enabling/disabling layer
	};
}
