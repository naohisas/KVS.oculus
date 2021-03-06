#include "BundledParallelCoordinates3DRenderer.h"
#include <kvs/OpenGL>


namespace
{

inline kvs::Vec3 Curve(
    const float t,
    const kvs::Vec3& p0,
    const kvs::Vec3& p1,
    const kvs::Vec3& p2,
    const kvs::Vec3& p3 )
{
    // Catmull-Rom with fist 3 points
    const kvs::Vec3 b = p3 - 3.0f * p2 + 3.0f * p1 - p0;
    const kvs::Vec3 c = 3.0f * p2 - 6.0f * p1 + 3.0f * p0;
    const kvs::Vec3 d = 3.0f * p1 - 3.0f * p0;
    const kvs::Vec3 e = p0;
    return ( b * t * t * t ) + ( c * t * t ) + ( d * t ) + e;
}

}

namespace local
{

BundledParallelCoordinates3DRenderer::BundledParallelCoordinates3DRenderer():
    m_reduced_plane_scale( 1.0f ),
    m_curve_size( 1.0f ),
    m_bundled_position( 0 ),
    m_bundled_line_size( 1.0f ),
    m_bundled_line_color( kvs::RGBColor::Black() ),
    m_nclusters( 0 ),
    m_clustered_colors( 256 )
{
    m_clustered_colors.create();
}

void BundledParallelCoordinates3DRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::TableObject* table = kvs::TableObject::DownCast( object );

    BaseClass::startTimer();
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );

    // Anti-aliasing.
    if ( BaseClass::isEnabledAntiAliasing() )
    {
        kvs::OpenGL::Enable( GL_LINE_SMOOTH );
        kvs::OpenGL::Enable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    }

    kvs::OpenGL::Enable( GL_DEPTH_TEST );

    const float dpr = camera->devicePixelRatio();
    //BaseClass::drawLines( table, dpr );
    /*if ( m_nclusters > 0 )
            {
                const float value = static_cast<float>( m_clustered_color_ids.at(i) );
                const kvs::RGBColor color = m_clustered_colors.at( value );
                local::ParallelCoordinates3DRenderer::setPointColor( color );
            }*/
    BaseClass::drawPoints( table, dpr );
    this->draw_bundled_lines( table, dpr );
    //this->draw_colored_points( table, dpr );

    BaseClass::stopTimer();
}

/*void BundledParallelCoordinates3DRenderer::draw_colored_points( const kvs::TableObject* table, const float dpr )
{
    // Rounded shape.
    kvs::OpenGL::Enable( GL_POINT_SMOOTH );

    kvs::OpenGL::SetPointSize( m_point_size * dpr );
    kvs::OpenGL::Begin( GL_POINTS );

    const float y_min_coord = table->minObjectCoord().y();
    const float y_max_coord = table->maxObjectCoord().y();
    const float z_min_coord = table->minObjectCoord().z();
    const float z_max_coord = table->maxObjectCoord().z();

    const size_t nplanes = table->numberOfColumns() / 2;
    const float x_stride = (float)nplanes * 0.5f * ( table->maxObjectCoord().x() - table->minObjectCoord().x() ) / ( nplanes - 1 );
    float x_coord =(float)nplanes * 0.5f * table->minObjectCoord().x();
    for ( size_t i = 0; i < nplanes; i++, x_coord += x_stride )
    {
        const kvs::ValueArray<float>& y_values = table->column( 2 * i + 0 ).asValueArray<float>();
        const kvs::ValueArray<float>& z_values = table->column( 2 * i + 1 ).asValueArray<float>();

        const float y_min_value = table->minValue( 2 * i + 0 );
        const float y_max_value = table->maxValue( 2 * i + 0 );
        const float z_min_value = table->minValue( 2 * i + 1 );
        const float z_max_value = table->maxValue( 2 * i + 1 );

        const size_t npoints = y_values.size();
        for ( size_t j = 0; j < npoints; j++ )
        {
            const float normalized_y_coord = ( y_values[j] - y_min_value ) / ( y_max_value - y_min_value );
            const float normalized_z_coord = ( z_values[j] - z_min_value ) / ( z_max_value - z_min_value );
            const float y_coord = ( y_max_coord - y_min_coord ) * normalized_y_coord + y_min_coord;
            const float z_coord = ( z_max_coord - z_min_coord ) * normalized_z_coord + z_min_coord;

            if ( m_nclusters > 0 )
            {
                const float value = static_cast<float>( m_clustered_color_ids.at(i) );
                const kvs::RGBColor color = m_clustered_colors.at( value );
                kvs::OpenGL::Color( color );
            }
            else
            {
                kvs::OpenGL::Color( m_bundled_line_color );
            }

            kvs::OpenGL::Vertex( x_coord, y_coord, z_coord );
        }
    }
    kvs::OpenGL::End();
}*/

void BundledParallelCoordinates3DRenderer::draw_bundled_lines( const kvs::TableObject* table, const float dpr )
{
    const float y_min_coord = table->minObjectCoord().y();
    const float y_max_coord = table->maxObjectCoord().y();
    const float z_min_coord = table->minObjectCoord().z();
    const float z_max_coord = table->maxObjectCoord().z();

    if ( m_nclusters > 0 )
    {
        m_clustered_colors.setRange( 0.0f, m_nclusters + 1.0f );
    }

    const size_t nlines = table->numberOfRows();
    for ( size_t i = 0; i < nlines; i++ )
    {
        kvs::OpenGL::SetLineWidth( m_bundled_line_size * dpr );
        kvs::OpenGL::Begin( GL_LINE_STRIP );
        if ( m_nclusters > 0 )
        {
            const float value = static_cast<float>( m_clustered_color_ids.at(i) );
            const kvs::RGBColor color = m_clustered_colors.at( value );
            kvs::OpenGL::Color( color );
        }
        else
        {
            kvs::OpenGL::Color( m_bundled_line_color );
        }

        const size_t nplanes = table->numberOfColumns() / 2;
        const float x_stride = (float)nplanes * 0.5f * ( table->maxObjectCoord().x() - table->minObjectCoord().x() ) / ( nplanes - 1 );
        float x_coord = (float)nplanes * 0.5f * table->minObjectCoord().x();
        for ( size_t j = 0; j < nplanes; j++, x_coord += x_stride )
        {
            // Plane at j.
            const kvs::ValueArray<float>& y_values = table->column( 2 * j + 0 ).asValueArray<float>();
            const kvs::ValueArray<float>& z_values = table->column( 2 * j + 1 ).asValueArray<float>();
            const float y_min_value = table->minValue( 2 * j + 0 );
            const float y_max_value = table->maxValue( 2 * j + 0 );
            const float z_min_value = table->minValue( 2 * j + 1 );
            const float z_max_value = table->maxValue( 2 * j + 1 );
            const float normalized_y_coord = ( y_values[i] - y_min_value ) / ( y_max_value - y_min_value );
            const float normalized_z_coord = ( z_values[i] - z_min_value ) / ( z_max_value - z_min_value );
            const float y_coord = ( y_max_coord - y_min_coord ) * normalized_y_coord + y_min_coord;
            const float z_coord = ( z_max_coord - z_min_coord ) * normalized_z_coord + z_min_coord;

            kvs::OpenGL::Vertex( x_coord, y_coord, z_coord );

            if ( j == bundledPosition() )
            {
                // Bundled plane.
                const kvs::ValueArray<float>& y1_values = m_reduced_data[0];
                const kvs::ValueArray<float>& z1_values = m_reduced_data[1];
                const float y1_min_value = m_reduced_min_values[0];
                const float y1_max_value = m_reduced_max_values[0];
                const float z1_min_value = m_reduced_min_values[1];
                const float z1_max_value = m_reduced_max_values[1];
                const float normalized_y1_coord = ( y1_values[i] - y1_min_value ) / ( y1_max_value - y1_min_value );
                const float normalized_z1_coord = ( z1_values[i] - z1_min_value ) / ( z1_max_value - z1_min_value );
                const float y1_coord = ( y_max_coord - y_min_coord ) * normalized_y1_coord + y_min_coord;
                const float z1_coord = ( z_max_coord - z_min_coord ) * normalized_z1_coord + z_min_coord;
                const float x1_coord = x_coord + 0.5f * x_stride;
                const float y_cluster_center = ( y_max_coord - y_min_coord ) * ( m_cluster_centers.at(2 * m_clustered_color_ids.at(i) + 0) - y1_min_value ) / ( y1_max_value - y1_min_value ) + y_min_coord;
                const float z_cluster_center = ( z_max_coord - z_min_coord ) * ( m_cluster_centers.at(2 * m_clustered_color_ids.at(i) + 1) - z1_min_value ) / ( z1_max_value - z1_min_value ) + z_min_coord;
                const float y1_coord_center = ( y_max_coord - y_min_coord ) * 0.5f + y_min_coord;
                const float z1_coord_center = ( z_max_coord - z_min_coord ) * 0.5f + z_min_coord;
                const float x1_coord_center = x1_coord;

                // Plane at j + 1
                const kvs::ValueArray<float>& y2_values = table->column( 2 * ( j + 1 ) + 0 ).asValueArray<float>();
                const kvs::ValueArray<float>& z2_values = table->column( 2 * ( j + 1 ) + 1 ).asValueArray<float>();
                const float y2_min_value = table->minValue( 2 * ( j + 1 ) + 0 );
                const float y2_max_value = table->maxValue( 2 * ( j + 1 ) + 0 );
                const float z2_min_value = table->minValue( 2 * ( j + 1 ) + 1 );
                const float z2_max_value = table->maxValue( 2 * ( j + 1 ) + 1 );
                const float normalized_y2_coord = ( y2_values[i] - y2_min_value ) / ( y2_max_value - y2_min_value );
                const float normalized_z2_coord = ( z2_values[i] - z2_min_value ) / ( z2_max_value - z2_min_value );
                const float y2_coord = ( y_max_coord - y_min_coord ) * normalized_y2_coord + y_min_coord;
                const float z2_coord = ( z_max_coord - z_min_coord ) * normalized_z2_coord + z_min_coord;
                const float x2_coord = x_coord + x_stride;

                // Spline curve
                const kvs::Vec3 p0( x_coord, y_coord, z_coord );
                const kvs::Vec3 p0_D_R( x1_coord, y_coord, z_coord );
                const kvs::Vec3 p1( x1_coord, y1_coord, z1_coord );
                const kvs::Vec3 p1_center( x1_coord_center, y1_coord_center, z1_coord_center ); // scaling
                const kvs::Vec3 cluster_center( x1_coord_center, y_cluster_center, z_cluster_center);
                const kvs::Vec3 scaled_p1 = reducedPlaneScale() * ( cluster_center - p1_center ) + p1_center + p1 - cluster_center; // scaling
                const kvs::Vec3 scaled_p0( p0.x(), scaled_p1.y(), scaled_p1.z());
                const kvs::Vec3 p2( x2_coord, y2_coord, z2_coord );
                const kvs::Vec3 p2_D_R( x1_coord, y2_coord, z2_coord );
                const kvs::Vec3 scaled_p2( p2.x(), scaled_p1.y(), scaled_p1.z());
                const kvs::Vec3 resized_p1_0 = curveSize() * scaled_p1 + (1-curveSize()) * scaled_p0;
                const kvs::Vec3 resized_p1_2 = curveSize() * scaled_p1 + (1-curveSize()) * scaled_p2;
                const kvs::Vec3 control1 = (1-curveSize()) * p0 + curveSize() * p0_D_R;
                const kvs::Vec3 control2 = (1-curveSize()) * scaled_p0 + curveSize() * resized_p1_0;
                const kvs::Vec3 control3 = (1-curveSize()) * scaled_p2 + curveSize() * resized_p1_2;
                const kvs::Vec3 control4 = (1-curveSize()) * p2 + curveSize() * p2_D_R;
                const size_t ndivs = kvs::Math::Max(100.0f ,10 * reducedPlaneScale());
                const float step = 1.0f / ndivs;
                //kvs::OpenGL::Vertex(p0);
                for ( size_t i = 0; i < ndivs; i++ )
                {
                    //kvs::OpenGL::Vertex( ::Curve( i * step, p0, control1, control2, resized_p1_0 ) );
                    kvs::OpenGL::Vertex( ::Curve( i * step, p0, control1, control2, scaled_p1 ) );
                }
                //kvs::OpenGL::Vertex(scaled_p1);
                for ( size_t j = 0; j < ndivs; j++ )
                {
                    //kvs::OpenGL::Vertex( ::Curve( j * step, resized_p1_2, control3, control4, p2 ) );
                    kvs::OpenGL::Vertex( ::Curve( j * step, scaled_p1, control3, control4, p2 ) );
                }
                //kvs::OpenGL::Vertex(p2);
            }
        }

        kvs::OpenGL::End();
    }
}

} // end of namespace local
