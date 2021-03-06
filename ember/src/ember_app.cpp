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
            :application("Ember Editor", width, height)
        {
        }

        ~ember() = default;

        // Inherited via application
        virtual void init() override
        {
            const float width = static_cast<float>(window().width());
            const float height = static_cast<float>(window().height());
            m_imgui_layer = make_ref<editor_layer>(width, height);
            push_layer(m_imgui_layer);
        }

        virtual void deinit() override
        {
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
            if(e.key_code() == key_codes::KEY_ESCAPE)
            {
                exit();
            }
            return false;
        }
    };


    application *create_application()
    {
        return new ember(1280, 720);
    }

}