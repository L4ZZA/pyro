// --------- Entry Point --------------- 
#include "pyro/core/entry_point.h" 
#include "layer_2d.h" 
#include "layer_3d.h" 


class sandbox_app : public pyro::application
{
public:
    sandbox_app(uint32_t width, uint32_t height)
        :application({ "Sandbox", width, height })
    {
    }

    ~sandbox_app() = default;

    // Inherited via application
    virtual void init() override
    {
        const float width = static_cast<float>(window().width());
        const float height = static_cast<float>(window().height());
        m_imgui_layer = std::make_shared<layer_2d>(width,height);
        //push_layer(new layer_3d());  
        push_layer(m_imgui_layer);
    }

    virtual void deinit() override
    {
    }

    void on_event(pyro::event &event) override
    {
        application::on_event(event);

        pyro::event_dispatcher dispatcher(event);
        // dispatch event on window X pressed  
        dispatcher.dispatch<pyro::key_pressed_event>(BIND_EVENT_FN(sandbox_app::on_key_pressed));
    }

    bool on_key_pressed(pyro::key_pressed_event &event)
    {
        if(event.event_type() == pyro::e_event_type::key_pressed)
        {
            if(event.key_code() == pyro::key_codes::KEY_ESCAPE)
            {
                exit();
            }
            //PYRO_TRACE("{0}", static_cast<char>(e.key_code()));  
        }
        return false;
    }

private:
};


pyro::application *pyro::create_application()
{
    return new sandbox_app(1280, 720);
}