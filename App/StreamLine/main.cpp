#include <kvs/OpenGL>
#include <Lib/Oculus.h>
#include <Lib/HeadMountedDisplay.h>
#include <Lib/Application.h>
#include <Lib/Screen.h>
#include <iostream>

#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/LineObject>
#include <kvs/LineRenderer>
#include <kvs/Streamline>
#include <kvs/TornadoVolumeData>


int main( int argc, char** argv )
{
    kvs::oculus::Application app( argc, argv );
    /* Read volume data from the specified data file. If the data file is not
     * specified, scalar hydrogen volume data is created by using
     * kvs::HydrogenVolumeData class.
     */
    kvs::StructuredVolumeObject* volume = NULL;
    if ( argc > 1 ) volume = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else            volume = new kvs::TornadoVolumeData( kvs::Vector3ui( 32, 32, 32 ) );
    
    std::vector<kvs::Real32> v;
    kvs::Vector3i min_coord( 15, 15,  0 );
    kvs::Vector3i max_coord( 20, 20, 30 );
    for ( int k = min_coord.z(); k < max_coord.z(); k++ )
    {
        for ( int j = min_coord.y(); j < max_coord.y(); j++ )
        {
            for ( int i = min_coord.x(); i < max_coord.x(); i++ )
            {
                v.push_back( static_cast<kvs::Real32>(i) );
                v.push_back( static_cast<kvs::Real32>(j) );
                v.push_back( static_cast<kvs::Real32>(k) );
            }
        }
    }
    kvs::PointObject* point = new kvs::PointObject;
    point->setCoords( kvs::ValueArray<kvs::Real32>( v ) );
    
    const kvs::TransferFunction transfer_function( 256 );
    kvs::LineObject* object = new kvs::Streamline( volume, point, transfer_function );
    
    delete volume;
    delete point;

    
    kvs::oculus::Screen screen( &app );
   
    screen.registerObject( object);
    screen.show();
    
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
