#pragma once

#include "pyro/core/core.h"
#include "pyro/events/event.h"
#include "pyro/core/timestep.h"

namespace pyro
{
	/// \brief Interface/base-class from which implement the layers in your applicaiton.
	class PYRO_API layer
	{
	public:
		layer(std::string const & name = "Layer");
		virtual ~layer();

		/// \brief Runs when a layer is pushed onto the layer stack
		virtual void on_attach() {}
		/// \brief Runs when a layer is popped from the layer stack
		virtual void on_detach() {}
		/// \brief Runs when application calls the on_update method.
		/// Should happen every frame.
		virtual void on_update(const timestep& timestep) {}
		/// \brief Render method specific to imgui.
		virtual void on_render() const {}
		/// \brief Runs whenever an event gets sent to the layer.
		virtual void on_event(event& event) {}

		/// \brief Gives you the name of the layer [for debug]
		std::string const& name() const { return m_debug_name; }
		bool is_imgui() const { return m_imgui; }

	protected:
		std::string m_debug_name = "";

		/// \brief Flag to distinguish between imgui and pyro rendering calls.
		bool		m_imgui = false ;

		//TODO: enabling/disabling layer
	};
}
