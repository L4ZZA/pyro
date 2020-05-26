#pragma once
#include <pyro/events/event.h>
#include <pyro/renderer/camera.h>

    class scene
    {
    public:
        scene(pyro::ref<pyro::camera> const &camera);
        virtual ~scene() = default;

        virtual void init() = 0;
        virtual void deinit() = 0;
        virtual void on_update(pyro::timestep const &ts) = 0;
        virtual void on_render() const;
        virtual void on_imgui_render() = 0;
        virtual void on_event(pyro::event &e) = 0;

    protected:
        // this method will be called after between the scene setup calls.
        virtual void on_render_internal() const = 0;

    protected:
        pyro::ref<pyro::camera> m_camera;
    };
