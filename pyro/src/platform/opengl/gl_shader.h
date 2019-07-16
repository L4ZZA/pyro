#pragma once
#include "pyro/renderer/shader.h"

namespace pyro
{
    /// \brief OpenGL shader implementation.
    class gl_shader : public shader
    {
    public:
        gl_shader(const std::string& vertex_source, const std::string& fragment_source);
        ~gl_shader();

        void bind() const override;
        void unbind() const override;

        void set_uniform(const std::string& name, int32_t val) override; 
        void set_uniform(const std::string& name, float val) override; 
        void set_uniform(const std::string& name, const glm::vec2& vec) override; 
        void set_uniform(const std::string& name, const glm::vec3& vec) override; 
        void set_uniform(const std::string& name, const glm::vec4& vec) override; 
        void set_uniform(const std::string& name, const glm::mat4& mat) override; 

    private:
        uint32_t m_program_id{0};
    };
}
