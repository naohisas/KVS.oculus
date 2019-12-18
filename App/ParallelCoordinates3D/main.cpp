#include "Application.h"
#include "Screen.h"
#include "TouchController.h"
#include <kvs/TableObject>
#include <kvs/ValueTable>
#include <kvs/Csv>
#include <kvs/String>
#include <DimensionalityReduction/Lib/PrincipalComponentAnalysis.h>
#include <DimensionalityReduction/Lib/MultiDimensionalScaling.h>
#include "ParallelCoordinates3DAxis.h"
#include "ParallelCoordinates3DRenderer.h"
#include "BundledParallelCoordinates3DRenderer.h"


//#define BUNDLED_RANDOM
#define BUNDLED_PCA
//#define BUNDLED_MDS

#if defined( BUNDLED_RANDOM ) || defined( BUNDLED_PCA ) || defined( BUNDLED_MDS )
typedef local::BundledParallelCoordinates3DRenderer Renderer;
#else
typedef local::ParallelCoordinates3DRenderer Renderer;
#endif


template <typename T>
inline kvs::ValueTable<T> GenerateData( const size_t nrows, const size_t ncols )
{
    return kvs::ValueTable<T>::Random( nrows, ncols );
}

template <typename T>
inline kvs::ValueTable<T> ReadData( const std::string& filename )
{
    kvs::Csv csv( filename );

    const size_t nrows = csv.numberOfRows();
    const size_t ncols = csv.row(0).size();
    kvs::ValueTable<T> data( nrows, ncols );
    for ( size_t i = 0; i < nrows; i++ )
    {
        const kvs::Csv::Row& row = csv.row(i);
        for ( size_t j = 0; j < ncols; j++ )
        {
            data[j][i] = kvs::String::To<T>( row[j] );
        }
    }

    return data;
}


int main( int argc, char** argv )
{
    local::Application app( argc, argv );
    local::Screen screen( &app );
    local::TouchController controller( &screen );

    screen.show();

    const size_t nrows = 20;
    const size_t ncols = 4;
    kvs::ValueTable<float> data = ( argc > 1 ) ? ReadData<float>( argv[1] ) : GenerateData<float>( nrows, ncols );

    kvs::TableObject* object = new kvs::TableObject();
    object->setTable( data );

    const float x_scale = 1.0f;
    const kvs::Vec3 scale( x_scale, 1.0f, 1.0f );
    const kvs::Vec3 min_coord = object->minExternalCoord() * scale;
    const kvs::Vec3 max_coord = object->maxExternalCoord() * scale;
    object->setMinMaxExternalCoords( min_coord, max_coord );

    Renderer* renderer = new Renderer();
    renderer->enableAntiAliasing();
    renderer->setPointSize( 7 );
    renderer->setLineSize( 1 );
    renderer->setBundledLineSize( 2 );
    renderer->setPointColor( kvs::RGBColor::Black() );
    renderer->setLineColor( kvs::RGBColor::Black() );
    renderer->setBundledLineColor( kvs::RGBColor::Red() );

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
    axis->setAxisWidth( 3.0f );
    axis->setAxisColor( kvs::RGBColor::Black() );
    axis->setBackgroundColor( kvs::RGBAColor( kvs::RGBColor::White(), 1.0f ) );
    screen.registerObject( object, axis );

    return app.run();
}
