// --------- Entry Point ---------------
#include "main_layer.h"
#include "pyro/entry_point.h"


class sandbox_app : public pyro::application 
{ 
public: 
    sandbox_app() 
    { 
        push_layer(new example_layer()); 
    } 

    ~sandbox_app() = default; 

    void on_event(pyro::event& event) override
    { 
        application::on_event(event); 

        pyro::event_dispatcher dispatcher(event); 
        // dispatch event on window X pressed 
        dispatcher.dispatch<pyro::key_pressed_event>(BIND_EVENT_FN(sandbox_app::on_key_pressed)); 
    } 

    bool on_key_pressed(pyro::key_pressed_event& event) 
    { 
        if (event.event_type() == pyro::event_type_e::key_pressed) 
        { 
            if (event.key_code() == pyro::key_codes::KEY_ESCAPE) 
            { 
                application::exit(); 
            } 
            //PYRO_TRACE("{0}", static_cast<char>(e.key_code())); 
        } 
        return false; 
    } 
}; 


pyro::application* pyro::create_application() 
{ 
    return new sandbox_app(); 
}