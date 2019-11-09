﻿#pragma once
#include "pyro/renderer/camera.h"
#include "pyro/core/timestep.h"

#include "pyro/events/application_event.h"
#include "pyro/events/mouse_event.h"

namespace pyro
{
    //======== CAMERA CONTROLLER INTERFACE ========
    class camera_controller
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
        virtual ~camera_controller() = default;
		virtual void on_update(timestep ts) = 0;
        virtual void on_event(event &e) = 0;

        virtual pyro::camera &      camera() = 0;
        virtual pyro::camera const& camera() const = 0;

        virtual void zoom_level(float level) = 0;
        virtual float zoom_level() const = 0;

    protected:
    };
    
    //=========== 2D CAMERA CONTROLLER ============

    class orthographic_camera_controller final : public camera_controller
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
        orthographic_camera_controller(float aspect_ratio, bool rotation = false);
        void on_update(timestep ts) override;
        void on_event(event& e) override;
        pyro::camera& camera() override;
        pyro::camera const& camera() const override;

        void zoom_level(float level) override;
        float zoom_level() const override;

    private:
        void move(e_direction direction, timestep ts);
        void rotate(e_rotation rotation, e_axis rotation_axis, timestep ts);
        bool on_mouse_scrolled(mouse_scrolled_event& e);
        bool on_window_resized(window_resize_event &e);

    private:
        float m_aspect_ratio;
        float m_zoom_level;
        orthographic_camera m_camera;

        bool m_rotation;
        glm::vec3 m_camera_position;
        /// \brief Rotation in degrees in anti-clockwise direction.
        float m_camera_rotation;
        float m_camera_translation_speed;
        const float m_camera_rotation_speed;
    };
    
    //=========== 3D CAMERA CONTROLLER ============

    class perspective_camera_controller : public camera_controller
    {
    public:
        enum class e_control_type
        {
            first_person,
            editor
        };

        enum e_direction 
        { 
            forward = 0, 
            backward, 
            left, 
            right 
        };

    public:
        perspective_camera_controller(
            e_control_type control_type,
            float width, float height,
            float fov = 45.f,
            float near_z = 0.1f, float far_z = 100.f);
        
        void on_update(timestep ts) override;
        void on_event(event& e) override;
        pyro::camera& camera() override;
        pyro::camera const& camera() const override;
        void zoom_level(float level) override;
        float zoom_level() const override;

    private:
        void process_mouse(e_control_type control_type, float mouse_x, float mouse_y, bool constrain_pitch = true);
        void process_mouse_delta(float mouse_delta_x, float mouse_delta_y, bool constrain_pitch = true);
        void process_mouse_panning(float mouse_x, float mouse_y);
        void move(e_direction direction, timestep ts); 
        void rotate(e_rotation rotation, e_axis rotation_axis, timestep ts);

    private:    
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

        glm::vec3   m_front_vector{0.f}; 
        glm::vec3   m_up_vector{0.f,1.f,0.f}; 
        glm::vec3   m_righ_vector{0.f}; 
        glm::vec3   m_world_up_vector{0.f,1.f,0.f};

        perspective_camera m_camera;

        bool m_rotation;
        glm::vec3 m_camera_position;
        /// \brief Rotation in degrees in anti-clockwise direction.
        glm::vec3 m_camera_rotation;
        const float m_camera_translation_speed;
        const float m_camera_rotation_speed;

        inline static e_control_type s_control_type = e_control_type::first_person;
        /// \brief in degrees per second. 
        inline static const float s_mouse_sensitivity = 0.1f;
    
    };
}