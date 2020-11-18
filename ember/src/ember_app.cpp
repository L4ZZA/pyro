// --------- Entry Point ---------------
#include "pyro/core/entry_point.h"
#include "editor_layer.h"

namespace pyro
{

    // Level editor
    // [Ember, a small piece burning or glowing in a fire]
    class ember : public application
    {
    public:
        ember(uint32_t width, uint32_t height)
            :application({"Ember Editor", width, height})
        {
        }

        ~ember() = default;

        // Inherited via application
        virtual void init() override
        {
            const uint32_t width  = window().width();
            const uint32_t height = window().height();
            m_layer_2d = make_ref<editor_layer>(width, height);
            push_layer(m_layer_2d);
            m_imgui_layer = std::dynamic_pointer_cast<imgui_layer>(m_layer_2d);
        }

        void on_event(event &e) override
        {
            application::on_event(e);

            event_dispatcher dispatcher(e);
            // dispatch event on window X pressed 
            dispatcher.dispatch<key_pressed_event>(BIND_EVENT_FN(ember::on_key_pressed));
        }

        bool on_key_pressed(key_pressed_event &e)
        {
            if(e.key_code() == key_codes::ESCAPE)
            {
                exit();
            }
            return false;
        }
    private:
        ref<layer> m_layer_2d;
    };


    application *create_application()
    {
        return new ember(1600, 900);
    }

}