#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/TableObject>
#include <kvs/ValueTable>
#include "ParallelCoordinates3DRenderer.h"
#include "ParallelCoordinates3DAxis.h"


int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );
    screen.show();

    const size_t nrows = 20;
    const size_t ncols = 6;
    kvs::ValueTable<float> table = kvs::ValueTable<float>::Random( nrows, ncols );

    kvs::TableObject* object = new kvs::TableObject();
    object->setTable( table );

    local::ParallelCoordinates3DRenderer* renderer = new local::ParallelCoordinates3DRenderer();
    renderer->enableAntiAliasing();
    screen.registerObject( object, renderer );

    local::ParallelCoordinates3DAxis* axis = new local::ParallelCoordinates3DAxis();
    axis->enableAntiAliasing();
    screen.registerObject( object, axis );

    return app.run();
}
