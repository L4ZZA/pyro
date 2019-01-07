#include <pyro.h>

class example_layer : public pyro::layer
{
public:
    example_layer() : layer("example") {  }

    void on_update() override
    {
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
    }

    ~sandbox_app()
    {
        
    }
};


pyro::application * pyro::create_application()
{
    return new sandbox_app();
}