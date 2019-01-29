#include <kvs/HydrogenVolumeData>
#include <kvs/StructuredVolumeObject>
#include <kvs/PolygonObject>
#include <kvs/Isosurface>
#include <kvs/Bounds>
#include <Lib/Oculus.h>
#include <Lib/HeadMountedDisplay.h>
#include <Lib/Application.h>
#include <Lib/Screen.h>
#include <Lib/TouchController.h>


int main( int argc, char** argv )
{
    kvs::oculus::Application app( argc, argv );
    kvs::oculus::Screen screen( &app );

    kvs::Indent indent( 4 );
    std::cout << "SDK Info." << std::endl;
    std::cout << indent << "Description: " << kvs::oculus::Description() << std::endl;
    std::cout << indent << "Version: " << kvs::oculus::Version() << std::endl;
    screen.headMountedDisplay().print( std::cout << "HMD Info." << std::endl, indent );

    kvs::StructuredVolumeObject* volume = new kvs::HydrogenVolumeData( kvs::Vec3ui( 64, 64, 64 ) );
    const double i = kvs::Math::Mix( volume->minValue(), volume->maxValue(), 0.2 );
    const kvs::PolygonObject::NormalType n = kvs::PolygonObject::VertexNormal;
    const bool d = false;
    const kvs::TransferFunction t( 256 );
    kvs::PolygonObject* object = new kvs::Isosurface( volume, i, n, d, t );
    delete volume;

    screen.registerObject( object, new kvs::Bounds() );
    screen.registerObject( object );
    screen.show( true );

    kvs::oculus::TouchController controller( &screen );
    controller.setRotationFactor( 4000.0f );
    controller.setTranslationFactor( 4000.0f );
    controller.setScalingFactor( 25000.0f );

    return app.run();
}
