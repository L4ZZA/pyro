#pragma once
#include "glm/mat4x4.hpp"

namespace pyro
{
    /// \brief Shader (platform agnostic) interface. Implemented in platform/api_name folder (i.e. opengl)
    class shader
    {
    public:
        virtual ~shader() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void set_uniform(const std::string& name, int32_t val) = 0;
        virtual void set_uniform(const std::string& name, float val) = 0;
        virtual void set_uniform(const std::string& name, const glm::vec2& vec) = 0;
        virtual void set_uniform(const std::string& name, const glm::vec3& vec) = 0;
        virtual void set_uniform(const std::string& name, const glm::vec4& vec) = 0;
        virtual void set_uniform(const std::string& name, const glm::mat4& mat) = 0; 
    };
}
