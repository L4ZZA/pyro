#pragma once 

namespace pyro
{
	/// \brief 
	class graphics_context
	{
	public:
		virtual void init() = 0;
		virtual void swap_buffers() = 0;
	};
}
