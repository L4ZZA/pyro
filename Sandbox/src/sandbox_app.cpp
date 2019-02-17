#include <pyro.h>
#include "../../Pyro/external/GLFW/include/GLFW/glfw3.h"

class example_layer : public pyro::layer
{
public:
    example_layer() : layer("example") {  }

    void on_update() override
    {
        if(pyro::input::key_pressed(pyro::key_codes::KEY_TAB))
            PYRO_INFO("Tab pressed");
        //PYRO_INFO("ExampleLayer::on_update()");
    }

    void on_event(pyro::event& p_event) override
    {
        //PYRO_TRACE("{0}", p_event);
    }

};


class sandbox_app : public pyro::application
{
    public:
    sandbox_app()
    {
        push_layer(new example_layer());
        push_overlay(new pyro::imgui_layer());
    }

    ~sandbox_app()
    {
        
    }
};


pyro::application * pyro::create_application()
{
    return new sandbox_app();
}