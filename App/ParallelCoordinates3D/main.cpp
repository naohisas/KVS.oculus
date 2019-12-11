#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/TableObject>
#include <kvs/ValueTable>
#include "ParallelCoordinates3DAxis.h"
#include "ParallelCoordinates3DRenderer.h"
#include "BundledParallelCoordinates3DRenderer.h"

#define BUNDLED

#if defined( BUNDLED )
typedef local::BundledParallelCoordinates3DRenderer Renderer;
#else
typedef local::ParallelCoordinates3DRenderer Renderer;
#endif

int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );
    screen.show();

    const size_t nrows = 20;
    const size_t ncols = 4;
    kvs::ValueTable<float> data = kvs::ValueTable<float>::Random( nrows, ncols );

    kvs::TableObject* object = new kvs::TableObject();
    object->setTable( data );

    Renderer* renderer = new Renderer();
    renderer->enableAntiAliasing();

#if defined( BUNDLED )
    kvs::ValueTable<float> reduced_data = kvs::ValueTable<float>::Random( nrows, 2 );
    renderer->setReducedData( reduced_data );
    renderer->setBundledPosition( 0 );
#endif

    screen.registerObject( object, renderer );

    local::ParallelCoordinates3DAxis* axis = new local::ParallelCoordinates3DAxis();
    axis->enableAntiAliasing();
    screen.registerObject( object, axis );

    return app.run();
}
