// --------- Entry Point ---------------
#include "pyro/core/entry_point.h"
#include "layer_2d.h"

// Level editor
// [Ember, a small piece burning or glowing in a fire]
class ember : public pyro::application
{
public:
    ember(uint32_t width, uint32_t height)
        :application("Ember Editor",width,height)
    {
    }

    ~ember() = default;

    // Inherited via application
    virtual void init() override
    {
        const float width  = static_cast<float>(window().width());
        const float height = static_cast<float>(window().height());
        m_layer_2d = pyro::make_ref<layer_2d>(width, height);
        //push_layer(new layer_3d()); 
        push_layer(m_layer_2d);
    }

    virtual void deinit() override
    {
    }

    void on_event(pyro::event &e) override
    {
        application::on_event(e);

        pyro::event_dispatcher dispatcher(e);
        // dispatch event on window X pressed 
        dispatcher.dispatch<pyro::key_pressed_event>(BIND_EVENT_FN(ember::on_key_pressed));
    }

    bool on_key_pressed(pyro::key_pressed_event &e)
    {
        if(e.key_code() == pyro::key_codes::KEY_ESCAPE)
        {
            exit();
        }
        return false;
    }

private:
    pyro::ref<layer_2d> m_layer_2d;
};


pyro::application *pyro::create_application()
{
    return new ember(1280, 720);
}