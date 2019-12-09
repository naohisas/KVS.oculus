#include "ParallelCoordinates3DAxis.h"
#include <kvs/OpenGL>
#include <kvs/TableObject>


namespace local
{

ParallelCoordinates3DAxis::ParallelCoordinates3DAxis()
{
}

void ParallelCoordinates3DAxis::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
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

    this->draw_planes( table );

    BaseClass::stopTimer();
}

void ParallelCoordinates3DAxis::draw_planes( const kvs::TableObject* table )
{
    const kvs::RGBColor line_color = kvs::RGBColor::Black();
    const float line_width = 2.0f;

    const kvs::Vec3 min_coord = table->minObjectCoord();
    const kvs::Vec3 max_coord = table->maxObjectCoord();

    const size_t nplanes = table->numberOfColumns() / 2;
    const float x_stride = ( max_coord.x() - min_coord.x() ) / ( nplanes - 1 );
    float x_coord = min_coord.x();
    for ( size_t i = 0; i < nplanes; i++, x_coord += x_stride )
    {
        kvs::OpenGL::SetLineWidth( line_width );
        kvs::OpenGL::Begin( GL_LINE_LOOP );
        kvs::OpenGL::Color( line_color );
        kvs::OpenGL::Vertex( x_coord, min_coord.y(), min_coord.z() );
        kvs::OpenGL::Vertex( x_coord, max_coord.y(), min_coord.z() );
        kvs::OpenGL::Vertex( x_coord, max_coord.y(), max_coord.z() );
        kvs::OpenGL::Vertex( x_coord, min_coord.y(), max_coord.z() );
        kvs::OpenGL::End();
    }
}

} // end of namespace local
