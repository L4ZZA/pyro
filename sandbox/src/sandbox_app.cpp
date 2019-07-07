// --------- Entry Point ---------------
#include "pyro/entry_point.h"
#include "main_layer.h"

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


pyro::application* pyro::create_application()
{
    return new sandbox_app();
}