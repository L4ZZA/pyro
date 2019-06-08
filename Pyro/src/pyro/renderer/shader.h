#pragma once

namespace pyro
{
    /// \brief Shader interface. Implemented in platform/api_name folder (i.e. opengl)
    class shader
    {
    public:
        virtual ~shader() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;
    };
}