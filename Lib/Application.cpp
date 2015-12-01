#include "Application.h"
#include "Oculus.h"


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
        if ( !ovr_Initialize( NULL ) ) { return; }
    }
}

int Application::run()
{
    return true;
}

void Application::quit()
{
    ovr_Shutdown();
}

} // end of namespace oculus

} // end of namespace kvs
