#pragma once
#include "pyro/core/timestep.h"
#define GLM_FORCE_CTOR_INIT
#include "glm/glm.hpp"


namespace pyro
{
    //================= CAMERA INTERFACE =================
    class camera
    {
    public:
        virtual ~camera() = default;

        virtual void             projection_matrix(glm::mat4 const &mat) = 0;
        virtual glm::mat4 const &projection_matrix() const = 0;
        virtual void             view_matrix(glm::mat4 const &mat) = 0;
        virtual glm::mat4 const &view_matrix() const = 0;
        virtual glm::mat4 const &view_projection_matrix() const = 0;

        virtual void position(const glm::vec3 &pos) = 0;
        virtual glm::vec3 const &position() const = 0;

        virtual void rotation(const glm::vec3 &rot) = 0;
        virtual glm::vec3 const &rotation() const = 0;
    };

    //================= 2D CAMERA =================

    class orthographic_camera final : public camera
    {
    public:
        orthographic_camera(float left, float right, float bottom, float top);

        glm::vec3 const &position() const override { return m_position; }
        void position(const glm::vec3 &pos) override { m_position = pos; update_view_matrix(); }

        glm::vec3 const &rotation() const override { return m_rotation; }
        void rotation(const glm::vec3 &rot) override { m_rotation = rot; update_view_matrix(); }

        void projection_matrix(float left, float right, float bottom, float top);
        void projection_matrix(glm::mat4 const &mat) override;
        void view_matrix(glm::mat4 const &mat) override;
        glm::mat4 const &projection_matrix() const override { return m_projection_mat; }
        glm::mat4 const &view_matrix() const override { return m_view_mat; }
        glm::mat4 const &view_projection_matrix() const override { return m_view_projection_mat; }

    private:
        void update_view_matrix();

    private:
        glm::mat4   m_projection_mat{1};
        glm::mat4   m_view_mat{1};
        glm::mat4   m_view_projection_mat{1};

        glm::vec3   m_position{0};
        glm::vec3   m_rotation{0};
    };

    //================= 3D CAMERA =================

    class perspective_camera : public camera
    {
    public:
        enum e_rotation
        {
            clock_wise = 0,
            anticlock_wise
        };

        enum e_axis
        {
            x = 0,
            y,
            z
        };

        enum e_direction
        {
            forward = 0,
            backward,
            left,
            right
        };

    public:
        perspective_camera(
            float width, float height,
            float fov = 45.f,
            float near_z = 0.1f, float far_z = 100.f);

        glm::vec3 const &position() const override { return m_position; }
        void position(const glm::vec3 &pos) override { m_position = pos; }

        glm::vec3 const &rotation() const override { return m_rotation; }
        void rotation(const glm::vec3 &rot) override { m_rotation = rot; }

        void fov(float fov) { m_fov = fov; }
        float fov() const { return m_fov; }

        glm::vec3 const &front() const { return m_front_vector; }
        glm::vec3 const &right_v() const { return m_right_vector; }
        glm::vec3 const &up() const { return m_up_vector; }

        void projection_matrix(glm::mat4 const &mat) override;
        void view_matrix(glm::mat4 const &mat) override;
        glm::mat4 const &projection_matrix() const override;
        glm::mat4 const &view_matrix() const override;
        glm::mat4 const &view_projection_matrix() const override;

        void update_camera_vectors();
    private:
        void update_view_matrix();

    protected:
        glm::mat4   m_projection_mat;
        glm::mat4   m_view_mat;
        glm::mat4   m_view_projection_mat;

        glm::vec3   m_position;
        /// \brief Rotation in degrees in anti-clockwise direction.
        /// pitch -> rotation over x axis
        /// yaw -> rotation over y axis
        /// roll -> rotation over z axis
        glm::vec3   m_rotation;

        glm::vec3   m_front_vector;
        glm::vec3   m_up_vector;
        glm::vec3   m_right_vector;
        glm::vec3   m_world_up_vector;

        /// \brief  
        float m_aspect_ratio;
        /// \brief Field of view in degrees.  
        float m_fov;
        /// \brief Near clipping plane.  
        float m_near_plane;
        /// \brief ar clipping plane.  
        float m_far_plane;

        /// \brief in units per seconds.  
        inline static const float s_movement_speed = 2.5f;
        /// \brief in degrees per second.  
        inline static const float s_rotation_speed = 90.f;
    };

}
