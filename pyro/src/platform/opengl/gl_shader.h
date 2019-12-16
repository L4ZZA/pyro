#pragma once
#include "pyro/renderer/shader.h"

namespace pyro
{
    /// \brief OpenGL shader implementation.
    class gl_shader final : public shader
    {
    public:
        gl_shader(std::string const &file_path);
        gl_shader(std::string const &name, std::string const &file_path);
        gl_shader(std::string const &name, std::string const &vertex_source, std::string const &fragment_source);
        ~gl_shader();

        void bind() const override;
        void unbind() const override;
        std::string const &name() const override;

        void set_int(std::string const &name, int32_t val) override;
        void set_float(std::string const &name, float val) override;
        void set_float2(std::string const &name, const glm::vec2 &vec) override;
        void set_float3(std::string const &name, const glm::vec3 &vec) override;
        void set_float4(std::string const &name, const glm::vec4 &vec) override;
        void set_mat4(std::string const &name, const glm::mat4 &mat) override;

    private:
        void upload_uniform(std::string const &name, int32_t val) const;
        void upload_uniform(std::string const &name, float val) const;
        void upload_uniform(std::string const &name, const glm::vec2 &vec) const;
        void upload_uniform(std::string const &name, const glm::vec3 &vec) const;
        void upload_uniform(std::string const &name, const glm::vec4 &vec) const;
        void upload_uniform(std::string const &name, const glm::mat4 &mat) const;

        void compile(std::unordered_map<uint32_t, std::string> const &sources);
        static std::string read_file(std::string const &file_path);
        static std::unordered_map<uint32_t, std::string> pre_process(std::string const &source);
        static std::string extract_name(std::string const &file_path);
        int32_t get_uniform_location(std::string const &name) const;

    private:
        uint32_t m_program_id;
        std::string m_name;
        std::string m_file_path;
        mutable std::unordered_map<std::string, int32_t> m_uniform_cache;
    };
}
