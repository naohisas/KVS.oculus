#include <Lib/Application.h>
#include <Lib/Screen.h>
#include <Lib/TouchController.h>
#include <kvs/Bounds>
#include <kvs/PolygonObject>
#include <kvs/PolygonRenderer>
#include <kvs/PolygonImporter>

namespace
{
const kvs::Vec3 ObjectScale = kvs::Vec3( 0.1015, 0.1015, 0.5016 );
const kvs::Vec3 MinCoord = kvs::Vec3( 0, 0, 0 ) * ObjectScale;
const kvs::Vec3 MaxCoord = kvs::Vec3( 599, 599, 65 ) * ObjectScale;
const kvs::Vec3 MidCoord = ( MinCoord + MaxCoord ) * 0.5f;
}


int main( int argc, char** argv )
{
    kvs::oculus::Application app( argc, argv );
    kvs::oculus::Screen screen( &app );
    kvs::oculus::TouchController controller( &screen );
    screen.setBackgroundColor( kvs::RGBColor::White() );
    screen.show();

    kvs::PolygonObject* polygon_object = new kvs::PolygonImporter( "wt_N2_080930_02_AB_0030.kvsml" );
    polygon_object->setOpacity(255);
    polygon_object->setColor( kvs::RGBColor::Magenta() );
    polygon_object->setMinMaxExternalCoords( ::MinCoord, ::MaxCoord );
    polygon_object->setMinMaxObjectCoords( ::MinCoord, ::MaxCoord );
    kvs::PolygonRenderer* polygon_renderer = new kvs::PolygonRenderer();
    screen.registerObject( polygon_object, polygon_renderer );

    kvs::PolygonObject* polygon_object2 = new kvs::PolygonImporter( "wt_N2_080930_02_P2_0077.kvsml" );
    polygon_object2->setOpacity(255);
    polygon_object2->setColor( kvs::RGBColor::Cyan() );
    polygon_object2->setMinMaxExternalCoords( ::MinCoord, ::MaxCoord );
    polygon_object2->setMinMaxObjectCoords( ::MinCoord, ::MaxCoord );
    screen.registerObject( polygon_object2, polygon_renderer );

    screen.registerObject( polygon_object, new kvs::Bounds() );

    return app.run();
}
