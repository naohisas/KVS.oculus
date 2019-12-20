#include "ParallelCoordinates3DAxis.h"
#include <kvs/OpenGL>
#include <kvs/TableObject>
#include <algorithm>


namespace local
{

ParallelCoordinates3DAxis::ParallelCoordinates3DAxis():
    m_axis_width( 1.0f ),
    m_axis_color( kvs::RGBColor::Black() ),
    m_background_color( kvs::RGBColor::White(), 0.0f )
{
}

void ParallelCoordinates3DAxis::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::TableObject* table = kvs::TableObject::DownCast( object );

    // Initial position list of the axis planes
    if ( m_plane_positions.size() == 0 )
    {
        // Following processes are executed once.
        const size_t nplanes = table->numberOfColumns() / 2;
        const kvs::Vec3 min_coord = table->minObjectCoord();
        const kvs::Vec3 max_coord = table->maxObjectCoord();
        const float x_stride = ( max_coord.x() - min_coord.x() ) / ( nplanes - 1 );
        const float y_coord = ( max_coord.y() + min_coord.y() ) * 0.5f;
        const float z_coord = ( max_coord.z() + min_coord.z() ) * 0.5f;
        float x_coord = min_coord.x();
        for ( size_t i = 0; i < nplanes; i++, x_coord += x_stride )
        {
            m_plane_positions.push_back( kvs::Vec3( x_coord, y_coord, z_coord ) );
        }
    }

    // Visibility ordering of the axis planes
    {
        const kvs::Vec4 camera_position = kvs::OpenGL::ModelViewMatrix().inverted() * kvs::Vec4( camera->position(), 1.0f );
        std::sort(
            m_plane_positions.begin(),
            m_plane_positions.end(),
            [ camera_position ] ( const kvs::Vec3& a, const kvs::Vec3& b )
            {
                return ( camera_position.xyz() - a ).length() > ( camera_position.xyz() - b ).length();
            } );
    }

    BaseClass::startTimer();
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );

    // Anti-aliasing.
    if ( m_enable_anti_aliasing ) { kvs::OpenGL::Enable( GL_LINE_SMOOTH ); }

    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::Enable( GL_DEPTH_TEST );

    const float dpr = camera->devicePixelRatio();
    this->draw_planes( table, dpr );

    BaseClass::stopTimer();
}

void ParallelCoordinates3DAxis::draw_planes( const kvs::TableObject* table, const float dpr )
{
    const kvs::Vec3 min_coord = table->minObjectCoord();
    const kvs::Vec3 max_coord = table->maxObjectCoord();
    for ( size_t i = 0; i < m_plane_positions.size(); i++ )
    {
        const float x_coord = m_plane_positions[i].x();
        kvs::OpenGL::SetLineWidth( m_axis_width * dpr );
        kvs::OpenGL::Begin( GL_LINE_LOOP );
        kvs::OpenGL::Color( m_axis_color );
        kvs::OpenGL::Vertex( x_coord, min_coord.y(), min_coord.z() );
        kvs::OpenGL::Vertex( x_coord, max_coord.y(), min_coord.z() );
        kvs::OpenGL::Vertex( x_coord, max_coord.y(), max_coord.z() );
        kvs::OpenGL::Vertex( x_coord, min_coord.y(), max_coord.z() );
        kvs::OpenGL::End();

        kvs::OpenGL::Begin( GL_QUADS );
        kvs::OpenGL::Color( m_background_color );
        kvs::OpenGL::Vertex( x_coord, min_coord.y(), min_coord.z() );
        kvs::OpenGL::Vertex( x_coord, max_coord.y(), min_coord.z() );
        kvs::OpenGL::Vertex( x_coord, max_coord.y(), max_coord.z() );
        kvs::OpenGL::Vertex( x_coord, min_coord.y(), max_coord.z() );
        kvs::OpenGL::End();
    }
/*
    const kvs::Vec3 min_coord = table->minObjectCoord();
    const kvs::Vec3 max_coord = table->maxObjectCoord();

    const size_t nplanes = table->numberOfColumns() / 2;
    const float x_stride = ( max_coord.x() - min_coord.x() ) / ( nplanes - 1 );
    float x_coord = min_coord.x();
    for ( size_t i = 0; i < nplanes; i++, x_coord += x_stride )
    {
        kvs::OpenGL::SetLineWidth( m_axis_width * dpr );
        kvs::OpenGL::Begin( GL_LINE_LOOP );
        kvs::OpenGL::Color( m_axis_color );
        kvs::OpenGL::Vertex( x_coord, min_coord.y(), min_coord.z() );
        kvs::OpenGL::Vertex( x_coord, max_coord.y(), min_coord.z() );
        kvs::OpenGL::Vertex( x_coord, max_coord.y(), max_coord.z() );
        kvs::OpenGL::Vertex( x_coord, min_coord.y(), max_coord.z() );
        kvs::OpenGL::End();

        kvs::OpenGL::Begin( GL_QUADS );
        kvs::OpenGL::Color( m_background_color );
        kvs::OpenGL::Vertex( x_coord, min_coord.y(), min_coord.z() );
        kvs::OpenGL::Vertex( x_coord, max_coord.y(), min_coord.z() );
        kvs::OpenGL::Vertex( x_coord, max_coord.y(), max_coord.z() );
        kvs::OpenGL::Vertex( x_coord, min_coord.y(), max_coord.z() );
        kvs::OpenGL::End();
    }
*/
}

} // end of namespace local
