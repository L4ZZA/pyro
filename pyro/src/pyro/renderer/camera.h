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

        virtual const glm::mat4& projection_matrix() const = 0;
        virtual const glm::mat4& view_matrix() const = 0;
        virtual const glm::mat4& view_projection_matrix() const = 0;

        virtual void position(const glm::vec3& pos) = 0;
        virtual glm::vec3 position() const = 0;

        virtual void rotation(const glm::vec3& rot) = 0;
        virtual glm::vec3 rotation() const = 0;
    };

    //================= 2D CAMERA =================

    class orthographic_camera final : public camera
    {

    public:
        orthographic_camera(float left, float right, float bottom, float top);

        glm::vec3 position() const override { return m_position; }
        void position(const glm::vec3& pos) override { m_position = pos; update_view_matrix(); }

        glm::vec3 rotation() const override { return m_rotation; }
        void rotation(const glm::vec3& rot) override { m_rotation = rot; update_view_matrix(); }
        
        void projection_matrix(float left, float right, float bottom, float top);
        const glm::mat4& projection_matrix() const override { return m_projection_mat; }
        const glm::mat4& view_matrix() const override { return m_view_mat; }
        const glm::mat4& view_projection_matrix() const override { return m_view_projection_mat; }

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
        perspective_camera(
            float width, float height,  
            float fov = 45.f,  
            float near_z = 0.1f, float far_z = 100.f);

        glm::vec3 position() const override { return m_position; }
        void position(const glm::vec3& pos) override { m_position = pos; update_view_matrix(); }

        glm::vec3 rotation() const override { return m_rotation; }
        void rotation(const glm::vec3& rot) override { m_rotation = rot; update_view_matrix(); }

        const glm::mat4& projection_matrix() const override; 
        const glm::mat4& view_matrix() const override; 
        const glm::mat4& view_projection_matrix() const override;

    private: 
        void update_camera_vectors();
        void update_view_matrix(); 

    protected: 
        glm::mat4   m_projection_mat{1}; 
        glm::mat4   m_view_mat{1}; 
        glm::mat4   m_view_projection_mat{1}; 

        glm::vec3   m_position{0.f}; 
        /// \brief rotation angles for each axis in degrees. 
        glm::vec3   m_rotation{0.f};

        glm::vec3   m_front_vector{0.f}; 
        glm::vec3   m_up_vector{0.f,1.f,0.f}; 
        glm::vec3   m_righ_vector{0.f}; 
        glm::vec3   m_world_up_vector{0.f,1.f,0.f};
        // Default camera values
        const float YAW         = -90.0f;
        const float PITCH       =  0.0f;
        const float ZOOM        =  45.0f;

        float m_aspect_ratio;
        float m_zoom_level;

        /// \brief 
        float m_yaw = YAW; 
        /// \brief 
        float m_pitch = PITCH; 
        /// \brief Field of view in degrees. 
        float m_fov = ZOOM; 
        /// \brief Near clipping plane. 
        float m_near_plane = 0.1f; 
        /// \brief ar clipping plane. 
        float m_far_plane = 100.f;
    };

}
