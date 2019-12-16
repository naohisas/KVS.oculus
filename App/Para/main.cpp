//#include <kvs/glut/Application>
//#include <kvs/glut/Screen>
#include <Lib/Application.h>
#include <Lib/Screen.h>
#include <Lib/TouchController.h>
#include <kvs/LineObject>
#include <kvs/PointObject>
#include <kvs/glut/Axis3D>
#include <iostream>
#include <cstdio>
#include <ctime>

const size_t N = 10;

kvs::Real32 CoordArrayAxis[ 6 ] = {
      0.0,   0.0, 100.0, 
      0.0, 100.0,   0.0
};

kvs::Real32 CoordArrayAxis2[ 6 ] = {
    100.0,   0.0, 100.0, 
    100.0, 100.0,   0.0
};


kvs::Real32 CoordArrayPoints[ N * 6 ];

kvs::UInt8 ColorArrayAxis[ 42 ];

kvs::UInt8 ColorArrayPoints[ N * 6 ];

kvs::PointObject* CreateAxisObject()
{
    kvs::ValueArray<kvs::Real32> coords( CoordArrayAxis, 6 );
    kvs::ValueArray<kvs::UInt8> colors( ColorArrayAxis, 6 );

    kvs::PointObject* object = new kvs::PointObject();
    object->setCoords( coords );
    object->setColors( colors );
    object->setSize( 1 );

    return object;
}
kvs::PointObject* CreateAxisObject2()
{
    kvs::ValueArray<kvs::Real32> coords( CoordArrayAxis2, 6 );
    kvs::ValueArray<kvs::UInt8> colors( ColorArrayAxis, 6 );

    kvs::PointObject* object = new kvs::PointObject();
    object->setCoords( coords );
    object->setColors( colors );
    object->setSize( 1 );

    return object;
}

kvs::PointObject* CreatePointObject(int size)
{
    int i;
    std::srand((unsigned int)std::time(NULL));;
    for (i = 0; i < N; i++)
    {
        CoordArrayPoints[6*i] = 0;
        CoordArrayPoints[6*i+2] = rand() % 100;
        CoordArrayPoints[6*i+1] = rand() % 100;
        CoordArrayPoints[6*i+3] = 100;
        CoordArrayPoints[6*i+5] = CoordArrayPoints[6*i+2]; //Dimension = 3
        //CoordArrayPoints[6*i+5] = rand() % 100; //Dimension = 4
        CoordArrayPoints[6*i+4] = rand() % 100;
        ColorArrayPoints[6*i] = 255-255/N*i;
        ColorArrayPoints[6*i+2] = 255/N*i;
        ColorArrayPoints[6*i+1] = 0;
        ColorArrayPoints[6*i+3] = 255-255/N*i;
        ColorArrayPoints[6*i+5] = 255/N*i;
        ColorArrayPoints[6*i+4] = 0;
    }
    
    kvs::ValueArray<kvs::Real32> coords( CoordArrayPoints, N * 6 );
    kvs::ValueArray<kvs::UInt8> colors( ColorArrayPoints, N * 6 );
    
    kvs::PointObject* object = new kvs::PointObject();
    object->setCoords( coords );
    object->setColors( colors );
    object->setSize( size );
    
    return object;
}

kvs::UInt32 ConnectionArray[ N * 2 ];


kvs::LineObject* CreateLineObject(int size)
{
    int i;
    for (i = 0; i < N; i++)
    {
        ConnectionArray[2*i] = 2*i;
        ConnectionArray[2*i+1] = 2*i+1;
    }
    kvs::ValueArray<kvs::Real32> coords( CoordArrayPoints, N * 6 );
    kvs::ValueArray<kvs::UInt8> colors( ColorArrayPoints, N * 6 );
    kvs::ValueArray<kvs::UInt32> connections( ConnectionArray, N * 2);
    
    kvs::LineObject* object = new kvs::LineObject();
    object->setCoords( coords );
    object->setColors( colors );
    object->setConnections( connections );
    object->setSize( size );
    object->setLineType( kvs::LineObject::Segment );
    object->setColorType( kvs::LineObject::VertexColor );
    
    return object;
}

int main( int argc, char** argv )
{
    //kvs::glut::Application app( argc, argv );
    //kvs::glut::Screen screen( &app );
    kvs::oculus::Application app( argc, argv );
    kvs::oculus::Screen screen( &app );
    kvs::oculus::TouchController controller( &screen );

    kvs::PointObject* object1 = CreateAxisObject();
    kvs::PointObject* object2 = CreateAxisObject2();
    
    kvs::PointObject* object3 = CreatePointObject(5);
    
    kvs::LineObject* object4 = CreateLineObject(5);

    kvs::glut::Axis3D* axis = new kvs::glut::Axis3D();
    axis->setXLabel( "User" );
    screen.setTitle( "Para" );
    //screen.registerObject( object1 );
    screen.registerObject( object1, axis );
    screen.registerObject( object2, axis );
    screen.registerObject( object3 );
    screen.registerObject( object4 );
    
    screen.show();
    
    return app.run();
}