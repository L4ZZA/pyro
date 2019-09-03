#pragma once
#include "pyro/core/timestep.h"
#define GLM_FORCE_CTOR_INIT
#include "glm/glm.hpp"


namespace pyro
{
    class camera
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

    public:
        virtual ~camera() = default;

        virtual void on_update(const timestep& timestep) = 0;

        virtual const glm::mat4& projection_matrix() const = 0;
        virtual const glm::mat4& view_matrix() const = 0;
        virtual const glm::mat4& view_projection_matrix() const = 0;
        virtual float movement_speed() const = 0;
        virtual float rotation_speed() const = 0;
    };

    class orthographic_camera final : public camera
    {
    public: 
        enum e_direction 
        { 
            up = 0, 
            down, 
            left, 
            right 
        }; 

    public:
        orthographic_camera(float left, float right, float bottom, float top);

        void on_update(const timestep& timestep) override{};
        void position(const glm::vec3& pos) { m_position = pos; update_view_matrix(); }

        float rotation() const { return m_rotation; }
        void rotation(float rot) { m_rotation = rot; update_view_matrix(); }

        const glm::mat4& projection_matrix() const override { return m_projection_mat; }
        const glm::mat4& view_matrix() const override { return m_view_mat; }
        const glm::mat4& view_projection_matrix() const override { return m_view_projection_mat; }
        
        float movement_speed() const override { return s_movement_speed; }
        float rotation_speed() const override { return s_rotation_speed; }

        void move(e_direction direction, timestep ts);
        void rotate(e_rotation rotation, timestep ts);

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

    class perspective_camera : public camera
    {
    public:
        enum e_direction 
        { 
            front = 0, 
            back, 
            left, 
            right 
        };

    public: 
        perspective_camera( 
            float width, float height, 
            const glm::vec3& start_position = {0.f,0.f,3.f},  
            float fov = 45.f,  
            float near_z = 0.1f, float far_z = 100.f); 

        void on_update(const timestep& timestep) override;
        const glm::mat4& projection_matrix() const override; 
        const glm::mat4& view_matrix() const override; 
        const glm::mat4& view_projection_matrix() const override; 

        float movement_speed() const override { return s_movement_speed; } 
        float rotation_speed() const override { return s_rotation_speed; } 

        void move(e_direction direction, timestep ts); 
        void rotate(e_rotation rotation, e_axis rotation_axis, timestep ts);

    private: 
        void update_view_matrix(); 

    private: 
        glm::mat4   m_projection_mat{1}; 
        glm::mat4   m_view_mat{1}; 
        glm::mat4   m_view_projection_mat{1}; 

        glm::vec3   m_position{0}; 
        /// \brief rotation angles for each axis in degrees. 
        glm::vec3   m_rotation_angle{0.f}; 

        /// \brief 
        float m_aspect_ratio{1}; 
        /// \brief Field of view in degrees. 
        float m_fov{45.f}; 
        /// \brief Near clipping plane. 
        float m_near_cplane{0.1f}; 
        /// \brief ar clipping plane. 
        float m_far_cplane{100.f}; 

        /// \brief in units per seconds. 
        inline static const float s_movement_speed = 1.f; 
        /// \brief in degrees per second. 
        inline static const float s_rotation_speed = 90.f;
    };
}
