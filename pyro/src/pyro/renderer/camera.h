#pragma once
#define GLM_FORCE_CTOR_INIT
#include "glm/glm.hpp"

namespace pyro
{
    class camera
    {
    public:
        virtual ~camera() = default;

        virtual const glm::mat4& projection_matrix() const = 0;
        virtual const glm::mat4& view_matrix() const = 0;
        virtual const glm::mat4& view_projection_matrix() const = 0;

    };

    class orthographic_camera final : public camera
    {
    public:
        orthographic_camera(float left, float right, float bottom, float top);

        glm::vec3 position() const { return m_position; }
        void position(const glm::vec3& pos) { m_position = pos; update_view_matrix(); }

        float rotation() const { return m_rotation; }
        void rotation(float rot) { m_rotation = rot; update_view_matrix(); }

        const glm::mat4& projection_matrix() const override { return m_projection_mat; }
        const glm::mat4& view_matrix() const override { return m_view_mat; }
        const glm::mat4& view_projection_matrix() const override { return m_view_projection_mat; }

    private:
        void update_view_matrix();

    private:
        glm::mat4   m_projection_mat{ 1 };
        glm::mat4   m_view_mat{ 1 };
        glm::mat4   m_view_projection_mat{ 1 };

        glm::vec3   m_position{ 0 };
        float       m_rotation{ 0 };
    };

    class perspective_camera
    {
    public:

    };
}
