#pragma once
#include <pyro/events/event.h>
#include <pyro/renderer/camera.h>

    class scene
    {
    public:
        scene(pyro::ref<pyro::camera_base> const &camera);
        virtual ~scene() = default;

        virtual void init() = 0;
        virtual void deinit() = 0;
        virtual void on_update(pyro::timestep const &ts) = 0;
        virtual void on_render() const = 0;
        virtual void on_imgui_render() = 0;
        virtual void on_event(pyro::event &e) = 0;

    protected:
        pyro::ref<pyro::camera_base> m_camera;
    };
