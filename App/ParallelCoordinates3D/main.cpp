//#include <kvs/glut/Application>
//#include <kvs/glut/Screen>
#include "Application.h"
#include "Screen.h"
#include "TouchController.h"
#include <kvs/TableObject>
#include <kvs/ValueTable>
#include "ParallelCoordinates3DAxis.h"
#include "ParallelCoordinates3DRenderer.h"
#include "BundledParallelCoordinates3DRenderer.h"
#include <DimensionalityReduction/Lib/PrincipalComponentAnalysis.h>
#include <DimensionalityReduction/Lib/MultiDimensionalScaling.h>

//#define BUNDLED_RANDOM
#define BUNDLED_PCA
//#define BUNDLED_MDS

#if defined( BUNDLED_RANDOM ) || defined( BUNDLED_PCA ) || defined( BUNDLED_MDS )
typedef local::BundledParallelCoordinates3DRenderer Renderer;
#else
typedef local::ParallelCoordinates3DRenderer Renderer;
#endif

int main( int argc, char** argv )
{
//    kvs::glut::Application app( argc, argv );
//    kvs::glut::Screen screen( &app );
    local::Application app( argc, argv );
    local::Screen screen( &app );
    local::TouchController controller( &screen );

    screen.show();

    const size_t nrows = 20;
    const size_t ncols = 4;
    kvs::ValueTable<float> data = kvs::ValueTable<float>::Random( nrows, ncols );

    kvs::TableObject* object = new kvs::TableObject();
    object->setTable( data );

    Renderer* renderer = new Renderer();
    renderer->enableAntiAliasing();

#if defined( BUNDLED_RANDOM )
    kvs::ValueTable<float> reduced_data = kvs::ValueTable<float>::Random( nrows, 2 );
    renderer->setReducedData( reduced_data );
    renderer->setBundledPosition( 0 );
#elif defined( BUNDLED_PCA )
    DimensionalityReduction::PrincipalComponentAnalysis<float> pca( data, 2 );
    kvs::ValueTable<float> reduced_data = pca.transform( data );
    renderer->setReducedData( reduced_data );
    renderer->setBundledPosition( 0 );
#elif defined( BUNDLED_MDS )
    DimensionalityReduction::MultiDimensionalScaling<float> mds( data, 2 );
    kvs::ValueTable<float> reduced_data = mds.transform( data );
    renderer->setReducedData( reduced_data );
    renderer->setBundledPosition( 0 );
#endif

    screen.registerObject( object, renderer );

    local::ParallelCoordinates3DAxis* axis = new local::ParallelCoordinates3DAxis();
    axis->enableAntiAliasing();
    screen.registerObject( object, axis );

    return app.run();
}
