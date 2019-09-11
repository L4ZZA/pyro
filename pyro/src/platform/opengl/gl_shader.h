#pragma once
#include "pyro/renderer/shader.h"

namespace pyro
{
    /// \brief OpenGL shader implementation.
    class gl_shader final : public shader
    {
    public:
        gl_shader(const std::string& file_path);
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
        std::string read_file(const std::string& file_path);
        std::unordered_map<uint32_t, std::string> pre_process(const std::string& source);
        void compile(const std::unordered_map<uint32_t, std::string>& sources);

    private:
        uint32_t m_program_id{0};
    };
}
