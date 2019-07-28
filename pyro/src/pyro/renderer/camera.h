#pragma once
#include "pyro/core/timestep.h"
#define GLM_FORCE_CTOR_INIT
#include "glm/glm.hpp"


namespace pyro
{
    class camera
    {
    public:
        enum e_direction
        {
            up = 0,
            down,
            left,
            right
        };

        enum e_rotation
        {
            clock_wise = 0,
            anticlock_wise
        };

    public:
        virtual ~camera() = default;

        virtual const glm::mat4& projection_matrix() const = 0;
        virtual const glm::mat4& view_matrix() const = 0;
        virtual const glm::mat4& view_projection_matrix() const = 0;
        virtual float movement_speed() const = 0;
        virtual float rotation_speed() const = 0;

        virtual void move(e_direction direction, timestep ts) = 0;
        virtual void rotate(e_rotation rotation, timestep ts) = 0;
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
        
        float movement_speed() const override { return s_movement_speed; }
        float rotation_speed() const override { return s_rotation_speed; }

        void move(e_direction direction, timestep ts) override;
        void rotate(e_rotation rotation, timestep ts) override;

    private:
        void update_view_matrix();

    private:
        glm::mat4   m_projection_mat{ 1 };
        glm::mat4   m_view_mat{ 1 };
        glm::mat4   m_view_projection_mat{ 1 };

        glm::vec3   m_position{ 0 };
        float       m_rotation{ 0 };

        /// \bief Movement speed in units per second
        inline static const float s_movement_speed = 1.0f;
        /// \brief Rotation speed in degrees per second
        inline static const float s_rotation_speed = 180.0f;
    };

    class perspective_camera
    {
    public:

    };
}
