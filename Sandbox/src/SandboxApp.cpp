#include <pyro.h>

class ExampleLayer : public pyro::layer
{
public:
    ExampleLayer() : layer("example") {  }

    void on_update() override
    {
        PYRO_INFO("ExampleLayer::on_update()");
    }

    void on_event(pyro::event& p_event) override
    {
        PYRO_TRACE("{0}", p_event);
    }

};


class SandboxApp : public pyro::application
{
    public:
    SandboxApp()
    {
        push_layer(new ExampleLayer());
    }

    ~SandboxApp()
    {
        
    }
};


pyro::application * pyro::create_application()
{
    return new SandboxApp();
}