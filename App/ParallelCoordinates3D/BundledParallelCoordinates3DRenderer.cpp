#include "BundledParallelCoordinates3DRenderer.h"
#include <kvs/OpenGL>


namespace local
{

BundledParallelCoordinates3DRenderer::BundledParallelCoordinates3DRenderer():
    m_reduced_plane_scale( 1.0f ),
    m_bundled_position( 0 ),
    m_bundled_line_size( 1.0f ),
    m_bundled_line_color( kvs::RGBColor::Black() )
{
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
    BaseClass::drawLines( table, dpr );
    BaseClass::drawPoints( table, dpr );
    this->draw_bundled_lines( table, dpr );

    BaseClass::stopTimer();
}

void BundledParallelCoordinates3DRenderer::draw_bundled_lines( const kvs::TableObject* table, const float dpr )
{
    const float y_min_coord = table->minObjectCoord().y();
    const float y_max_coord = table->maxObjectCoord().y();
    const float z_min_coord = table->minObjectCoord().z();
    const float z_max_coord = table->maxObjectCoord().z();

    const size_t nlines = table->numberOfRows();
    for ( size_t i = 0; i < nlines; i++ )
    {
        kvs::OpenGL::SetLineWidth( m_bundled_line_size * dpr );
        kvs::OpenGL::Begin( GL_LINE_STRIP );
        kvs::OpenGL::Color( m_bundled_line_color );

        const size_t nplanes = table->numberOfColumns() / 2;
        const float x_stride = ( table->maxObjectCoord().x() - table->minObjectCoord().x() ) / ( nplanes - 1 );
        float x_coord = table->minObjectCoord().x();
        for ( size_t j = 0; j < nplanes; j++, x_coord += x_stride )
        {
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

            if ( j == m_bundled_position )
            {
                const kvs::ValueArray<float>& y0_values = m_reduced_data[0];
                const kvs::ValueArray<float>& z0_values = m_reduced_data[1];
                const float y0_min_value = m_reduced_min_values[0];
                const float y0_max_value = m_reduced_max_values[0];
                const float z0_min_value = m_reduced_min_values[1];
                const float z0_max_value = m_reduced_max_values[1];
                const float normalized_y0_coord = ( y0_values[i] - y0_min_value ) / ( y0_max_value - y0_min_value );
                const float normalized_z0_coord = ( z0_values[i] - z0_min_value ) / ( z0_max_value - z0_min_value );
                const float y0_coord = ( y_max_coord - y_min_coord ) * normalized_y0_coord + y_min_coord;
                const float z0_coord = ( z_max_coord - z_min_coord ) * normalized_z0_coord + z_min_coord;
                const float x0_coord = x_coord + 0.5f * x_stride;
                kvs::OpenGL::Vertex( x0_coord, y0_coord, z0_coord );
            }
        }

        kvs::OpenGL::End();
    }
}

} // end of namespace local
