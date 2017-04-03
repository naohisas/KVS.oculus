#include <kvs/OpenGL>
#include <Lib/Oculus.h>
#include <Lib/HeadMountedDisplay.h>
#include <Lib/Application.h>
#include <Lib/Screen.h>
#include <iostream>

#include <kvs/HydrogenVolumeData>
#include <kvs/StructuredVolumeObject>
#include <kvs/PolygonObject>
#include <kvs/Isosurface>
#include <kvs/Bounds>

void Isosurface( kvs::glut::Screen& screen )
{
  kvs::StructuredVolumeObject* volume = NULL;

      volume = new kvs::HydrogenVolumeData( kvs::Vec3ui( 64, 64, 64 ) );


    
    const double i = kvs::Math::Mix( volume->minValue(), volume->maxValue(), 0.2 );
    const kvs::PolygonObject::NormalType n = kvs::PolygonObject::VertexNormal;
    const bool d = false;
    const kvs::TransferFunction t( 256 );
    kvs::PolygonObject* object = new kvs::Isosurface( volume, i, n, d, t );
    delete volume;

    screen.registerObject( object, new kvs::Bounds() );
    screen.registerObject( object );
}

#include <kvs/RayCastingRenderer>

void VolumeRendering( kvs::glut::Screen& screen )
{
    kvs::StructuredVolumeObject* object = new kvs::HydrogenVolumeData( kvs::Vec3ui( 64, 64, 64 ) );
    kvs::StructuredVolumeObject* m_volume = new kvs::StructuredVectorToScalar(volume);
//    kvs::glsl::RayCastingRenderer* renderer = new kvs::glsl::RayCastingRenderer();
    kvs::RayCastingRenderer* renderer = new kvs::RayCastingRenderer();

    screen.registerObject( object, new kvs::Bounds() );
    screen.registerObject( object, renderer );
}

int main( int argc, char** argv )
{
    kvs::oculus::Application app( argc, argv );
    kvs::oculus::Screen screen( &app );
    screen.show();
    

    Isosurface( screen );
//    VolumeRendering( screen );

//    screen.show();

    return app.run();
/*
    std::cout << "Description: " << kvs::oculus::Description() << std::endl;
    std::cout << "Version: " << kvs::oculus::Version() << std::endl;

    kvs::oculus::Application app( argc, argv );
    app.run();

    kvs::oculus::HeadMountedDisplay hmd;
    hmd.createDebug( ovrHmd_DK2 );

    std::cout << "Product Name: " << hmd.productName() << std::endl;
    std::cout << "Manufacturer: " << hmd.manufacturer() << std::endl;
    std::cout << "Vendor ID: " << hmd.vendorId() << std::endl;
    std::cout << "Product ID: " << hmd.productId() << std::endl;
    std::cout << "Resolution: " << kvs::oculus::ToVec2i( hmd.resolution() ) << std::endl;
    std::cout << "Window Position: " << kvs::oculus::ToVec2i( hmd.windowPosition() ) << std::endl;

    hmd.destroy();

    app.quit();

    return 0;
*/
}
