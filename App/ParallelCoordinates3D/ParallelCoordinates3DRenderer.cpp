#include "ParallelCoordinates3DRenderer.h"
#include <kvs/IgnoreUnusedVariable>
#include <kvs/OpenGL>


namespace local
{

ParallelCoordinates3DRenderer::ParallelCoordinates3DRenderer():
    m_enable_anti_aliasing( false )
{
}

void ParallelCoordinates3DRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::TableObject* table = kvs::TableObject::DownCast( object );

    BaseClass::startTimer();
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );

    // Anti-aliasing.
    if ( m_enable_anti_aliasing )
    {
        kvs::OpenGL::Enable( GL_LINE_SMOOTH );
        kvs::OpenGL::Enable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    }

    this->drawPoints( table );
    this->drawLines( table );

    BaseClass::stopTimer();
}

void ParallelCoordinates3DRenderer::drawPoints( const kvs::TableObject* table )
{
    const kvs::RGBColor point_color = kvs::RGBColor::Red();
    const float point_size = 5.0f;

    // Rounded shape.
    kvs::OpenGL::Enable( GL_POINT_SMOOTH );

    const float y_min_coord = table->minObjectCoord().y();
    const float y_max_coord = table->maxObjectCoord().y();
    const float z_min_coord = table->minObjectCoord().z();
    const float z_max_coord = table->maxObjectCoord().z();

    const size_t nplanes = table->numberOfColumns() / 2;
    const float x_stride = ( table->maxObjectCoord().x() - table->minObjectCoord().x() ) / ( nplanes - 1 );
    float x_coord = table->minObjectCoord().x();
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

            kvs::OpenGL::SetPointSize( point_size );
            kvs::OpenGL::Begin( GL_POINTS );
            kvs::OpenGL::Color( point_color );
            kvs::OpenGL::Vertex( x_coord, y_coord, z_coord );
            kvs::OpenGL::End();
        }
    }
}

void ParallelCoordinates3DRenderer::drawLines( const kvs::TableObject* table )
{
    const kvs::RGBColor line_color = kvs::RGBColor::Red();
    const float line_width = 1.0f;

    const float y_min_coord = table->minObjectCoord().y();
    const float y_max_coord = table->maxObjectCoord().y();
    const float z_min_coord = table->minObjectCoord().z();
    const float z_max_coord = table->maxObjectCoord().z();

    const size_t nlines = table->numberOfRows();
    for ( size_t i = 0; i < nlines; i++ )
    {
        kvs::OpenGL::SetLineWidth( line_width );
        kvs::OpenGL::Begin( GL_LINE_STRIP );
        kvs::OpenGL::Color( line_color );

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
        }

        kvs::OpenGL::End();
    }
}

} // end of namespace local
