#include <Lib/Oculus.h>
#include <Lib/HeadMountedDisplay.h>
#include <Lib/Application.h>
#include <iostream>


int main( int argc, char** argv )
{
    std::cout << "Description: " << kvs::oculus::Description() << std::endl;
    std::cout << "Version: " << kvs::oculus::Version() << std::endl;
    std::cout << "Detected HMDs: " << kvs::oculus::HeadMountedDisplay::Detect() << std::endl;

    kvs::oculus::Application app( argc, argv );
    app.run();

    std::cout << "Detected HMDs: " << kvs::oculus::HeadMountedDisplay::Detect() << std::endl;

    app.quit();

    return 0;
}
