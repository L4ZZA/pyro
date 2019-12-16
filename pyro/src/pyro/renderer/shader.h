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
        virtual std::string const &name() const = 0;

        virtual void set_int(std::string const &name, int32_t val) = 0;
        virtual void set_float(std::string const &name, float val) = 0;
        virtual void set_float2(std::string const &name, const glm::vec2 &vec) = 0;
        virtual void set_float3(std::string const &name, const glm::vec3 &vec) = 0;
        virtual void set_float4(std::string const &name, const glm::vec4 &vec) = 0;
        virtual void set_mat4(std::string const &name, const glm::mat4 &mat) = 0;

        static ref<shader> create(std::string const &file_path);
        static ref<shader> create(std::string const &name, std::string const &file_path);
        static ref<shader> create(std::string const &name, std::string const &vertex_source, std::string const &fragment_source);
    };

    class shader_library final
    {
    public:
        void add(ref<shader> const &shader);
        ref<shader> load(std::string const &filepath);
        ref<shader> load(std::string const &name, std::string const &filepath);

        ref<shader> get(std::string const &name);

        bool exists(std::string const &name);
    private:
        std::unordered_map<std::string, ref<shader>> m_shaders;
    };
}
