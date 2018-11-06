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

int main()
{
    auto sandbox = new SandboxApp();
    sandbox->run();
    delete sandbox;
}
