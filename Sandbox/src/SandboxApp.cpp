#include <pyro.h>

class SandboxApp : public pyro::application
{
    public:
    SandboxApp()
    {
        
    }

    ~SandboxApp()
    {
        
    }
};


pyro::application * pyro::create_application()
{
    return new SandboxApp();
}