#include "Application.h"
#include "OVR.h"

namespace kvs
{

namespace oculus
{

Application::Application( int argc, char** argv ):
    kvs::glut::Application( argc, argv )
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
//    return true;
    return kvs::glut::Application::run();
}

void Application::quit()
{
    kvs::oculus::Shutdown();
    kvs::glut::Application::quit();
}

} // end of namespace oculus

} // end of namespace kvs
