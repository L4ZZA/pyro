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
        virtual const std::string& name() const = 0;

        virtual void set_int(const std::string& name, int32_t val) = 0;
        virtual void set_float(const std::string& name, float val) = 0;
        virtual void set_vec2(const std::string& name, const glm::vec2& vec) = 0;
        virtual void set_vec3(const std::string& name, const glm::vec3& vec) = 0;
        virtual void set_vec4(const std::string& name, const glm::vec4& vec) = 0;
        virtual void set_mat4(const std::string& name, const glm::mat4& mat) = 0; 

        static ref<shader> create(const std::string& file_path);
        static ref<shader> create(const std::string& name, const std::string& vertex_source, const std::string& fragment_source);
    };

    class shader_library final
    {
    public:
        static void add(const ref<shader>& shader);
        static ref<shader> load(const std::string& filepath);
        static ref<shader> load(const std::string& name, const std::string& filepath);

        static ref<shader> get(const std::string& name);

        static bool exists(const std::string& name);
    private:
        inline static std::unordered_map<std::string, ref<shader>> m_shaders;
    };
}
