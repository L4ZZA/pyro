#pragma once 

namespace pyro
{
    enum class PYRO_API e_renderer_api
    {
        none = 0, opengl = 1, directx11 = 2
    };

	/// Graphics context (platform agnostic) interface
	class PYRO_API graphics_context
	{
	public:
		virtual void init() = 0;
		virtual void swap_buffers() = 0;
	};
}
