#include "Application.h"
#include "OVR.h"

namespace kvs
{

namespace oculus
{

Application::Application( int argc, char** argv ):
    kvs::ApplicationBase( argc, argv )
{
    static bool flag = true;
    if ( flag )
    {
        flag = false;
        if ( !kvs::oculus::Initialize( NULL ) ) { return; }
    }
}

int Application::run()
{
    return true;
}

void Application::quit()
{
    kvs::oculus::Shutdown();
}

} // end of namespace oculus

} // end of namespace kvs
