#pragma once 

namespace pyro
{
	/// Graphics context (platform agnostic) interface
	class PYRO_API graphics_context
	{
	public:
		virtual void init() = 0;
		virtual void swap_buffers() = 0;
	};
}
