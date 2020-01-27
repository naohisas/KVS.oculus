#include "Application.h"
#include "Screen.h"
//#include "TouchController.h"
#include <kvs/TableObject>
#include <kvs/ValueTable>
#include <kvs/Csv>
#include <kvs/String>
#include <kvs/AdaptiveKMeans>
#include <kvs/FastKMeans>
#include <kvs/ColorMap>
#include <DimensionalityReduction/Lib/PrincipalComponentAnalysis.h>
#include <DimensionalityReduction/Lib/MultiDimensionalScaling.h>
#include "ParallelCoordinates3DAxis.h"
#include "ParallelCoordinates3DRenderer.h"
#include "BundledParallelCoordinates3DRenderer.h"
#include "MyTouchController.h"


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
    local::MyTouchController controller( &screen );

    screen.setBackgroundColor( kvs::RGBColor::White() );
    screen.show();

    const size_t nrows = 20;
    const size_t ncols = 8;
    kvs::ValueTable<float> data = ( argc > 1 ) ? ReadData<float>( argv[1] ) : GenerateData<float>( nrows, ncols );

    kvs::TableObject* object = new kvs::TableObject();
    object->setTable( data );
    object->setName( "Object" );

    const float x_scale = 1.5f;
    const kvs::Vec3 scale( x_scale, 1.0f, 1.0f );
    const kvs::Vec3 min_coord = object->minExternalCoord() * scale;
    const kvs::Vec3 max_coord = object->maxExternalCoord() * scale;
    object->setMinMaxExternalCoords( min_coord, max_coord );

    Renderer* renderer = new Renderer();
    renderer->setName( "Renderer" );
    renderer->enableAntiAliasing();
    renderer->setPointSize( 7 );
    renderer->setLineSize( 1.5 );
    renderer->setBundledLineSize( 1.5 );
    renderer->setPointColor( kvs::RGBColor::Black() );
    renderer->setLineColor( kvs::RGBColor::Black() );
    renderer->setBundledLineColor( kvs::RGBColor::Red() );

    kvs::ColorMap cmap( 256 );
    /*
    cmap.setRange( 0, 1 );
    cmap.addPoint( 0.0, kvs::RGBColor( 215,  25,  28 ) );
    cmap.addPoint( 0.2, kvs::RGBColor( 253,  17,  97 ) );
    cmap.addPoint( 0.4, kvs::RGBColor( 255, 255, 191 ) );
    cmap.addPoint( 0.6, kvs::RGBColor( 171, 221, 164 ) );
    cmap.addPoint( 0.8, kvs::RGBColor(  43, 131, 186 ) );
    cmap.addPoint( 1.0, kvs::RGBColor(  43, 131, 186 ) );
    */
    cmap.create();

#if defined( BUNDLED_RANDOM )
    kvs::ValueTable<float> reduced_data = kvs::ValueTable<float>::Random( nrows, 2 );
    renderer->setReducedData( reduced_data );
    renderer->setBundledPosition( 0 );
    renderer->setReducedPlaneScale( 3.0f );

//    kvs::AdaptiveKMeans kmeans;
//    kmeans.setInputTableData( reduced_data );
//    kmeans.run();
    kvs::FastKMeans kmeans;
    kmeans.setInputTableData( reduced_data );
    kmeans.setSeedingMethod( kvs::FastKMeans::SmartSeeding );
    kmeans.setNumberOfClusters( 4 );
    kmeans.run();

    renderer->setNumberOfClusters( kmeans.numberOfClusters() );
    renderer->setClusterColorIDs( kmeans.clusterIDs() );
    renderer->setClusterColors( cmap );

#elif defined( BUNDLED_PCA )
    DimensionalityReduction::PrincipalComponentAnalysis<float> pca( data, 2 );
    kvs::ValueTable<float> reduced_data = pca.transform( data );
    renderer->setReducedData( reduced_data );
    renderer->setBundledPosition( 1 );
    renderer->setReducedPlaneScale( 1.0f ); //scaling
    renderer->setCurveSize( 0.1f ); //0 <= CurveSize <= 1

//    kvs::AdaptiveKMeans kmeans;
//    kmeans.setInputTableData( reduced_data );
//    kmeans.run();
    kvs::FastKMeans kmeans;
    kmeans.setInputTableData( reduced_data );
    kmeans.setSeedingMethod( kvs::FastKMeans::SmartSeeding );
    kmeans.setNumberOfClusters( 4 );
    kmeans.run();

    renderer->setNumberOfClusters( kmeans.numberOfClusters() );
    renderer->setClusterColorIDs( kmeans.clusterIDs() );
    renderer->setClusterColors( cmap );

#elif defined( BUNDLED_MDS )
    DimensionalityReduction::MultiDimensionalScaling<float> mds( data, 2 );
    kvs::ValueTable<float> reduced_data = mds.transform( data );
    renderer->setReducedData( reduced_data );
    renderer->setBundledPosition( 0 );
    renderer->setReducedPlaneScale( 3.0f );

//    kvs::AdaptiveKMeans kmeans;
//    kmeans.setInputTableData( reduced_data );
//    kmeans.run();
    kvs::FastKMeans kmeans;
    kmeans.setInputTableData( reduced_data );
    kmeans.setSeedingMethod( kvs::FastKMeans::SmartSeeding );
    kmeans.setNumberOfClusters( 4 );
    kmeans.run();

    renderer->setNumberOfClusters( kmeans.numberOfClusters() );
    renderer->setClusterColorIDs( kmeans.clusterIDs() );
    renderer->setClusterColors( cmap );
#endif

    screen.registerObject( object, renderer );

    local::ParallelCoordinates3DAxis* axis = new local::ParallelCoordinates3DAxis();
    axis->enableAntiAliasing();
    axis->setAxisWidth( 2.0f );
    axis->setAxisColor( kvs::RGBColor::Black() );
    axis->setBackgroundColor( kvs::RGBAColor( kvs::RGBColor( 230, 230, 230 ), 0.8f ) );
    screen.registerObject( object, axis );

    return app.run();
}
