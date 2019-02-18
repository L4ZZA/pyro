#include <pyro.h>

class example_layer : public pyro::layer
{
public:
    example_layer() : layer("example") {  }

    void on_update() override
    {
        if(pyro::input::key_pressed(pyro::key_codes::KEY_TAB))
            PYRO_INFO("Tab pressed (poll)");
        //PYRO_INFO("ExampleLayer::on_update()");
    }

    void on_event(pyro::event& p_event) override
    {
        if(p_event.event_type() == pyro::event_type_e::key_pressed)
        {
            auto& e = dynamic_cast<pyro::key_pressed_event&>(p_event);
            if(e.key_code() == pyro::key_codes::KEY_TAB)
                PYRO_TRACE("Tab pressed (event)");
            PYRO_TRACE("{0}", static_cast<char>(e.key_code()));
        }
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