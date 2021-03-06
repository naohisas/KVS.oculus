#include "ParallelCoordinates3DAxis.h"
#include <kvs/OpenGL>
#include <kvs/TableObject>
#include <algorithm>
#include <kvs/glut/GLUT>


namespace local
{

ParallelCoordinates3DAxis::ParallelCoordinates3DAxis():
    m_axis_width( 1.0f ),
    m_axis_color( kvs::RGBColor::Black() ),
    m_label_color( kvs::RGBColor::Black() ),
    m_value_color( kvs::RGBColor::Black() ),
    m_background_color( kvs::RGBColor::White(), 0.0f ),
    m_number_of_grid( 3 ),
    m_show_grids( true ),
    m_show_labels( true ),
    m_show_values( true )
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
        const kvs::Vec3 min_coord = (float)nplanes * 0.5f * table->minObjectCoord();
        const kvs::Vec3 max_coord = (float)nplanes * 0.5f * table->maxObjectCoord();
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
        //const kvs::Vec4 camera_position = kvs::OpenGL::ModelViewMatrix().inverted() * kvs::Vec4( camera->position(), 1.0f );
        /*
        std::sort(
            m_plane_positions.begin(),
            m_plane_positions.end(),
            [ camera_position ] ( const kvs::Vec3& a, const kvs::Vec3& b )
            {
                return ( camera_position.xyz() - a ).length() > ( camera_position.xyz() - b ).length();
            } );
            */
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
    this->draw_gridlines( table, dpr );
    this->draw_labels( table, dpr );

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
    
void ParallelCoordinates3DAxis::draw_gridlines( const kvs::TableObject* table, const float dpr )
{
    const kvs::Vec3 min_coord = table->minObjectCoord();
    const kvs::Vec3 max_coord = table->maxObjectCoord();
    float y_interval = ( max_coord.y() - min_coord.y() ) / (m_number_of_grid + 1);
    float z_interval = ( max_coord.z() - min_coord.z() ) / (m_number_of_grid + 1);
    if( m_show_grids )
    {
        for ( size_t i = 0; i < m_plane_positions.size(); i++ )
        {
            const float x_coord = m_plane_positions[i].x();
            kvs::OpenGL::SetLineWidth( m_axis_width * dpr * 0.5 );
            kvs::OpenGL::Color( m_axis_color );
            for ( size_t j = 0; j < m_number_of_grid; j++ )
            {
                kvs::OpenGL::Begin( GL_LINE_STRIP );
                kvs::OpenGL::Vertex( x_coord, min_coord.y(), min_coord.z() + z_interval * (j+1) );
                kvs::OpenGL::Vertex( x_coord, max_coord.y(), min_coord.z() + z_interval * (j+1) );
                kvs::OpenGL::End();
                kvs::OpenGL::Begin( GL_LINE_STRIP );
                kvs::OpenGL::Vertex( x_coord, min_coord.y() + y_interval * (j+1), min_coord.z() );
                kvs::OpenGL::Vertex( x_coord, min_coord.y() + y_interval * (j+1), max_coord.z() );
                kvs::OpenGL::End();
            }
        }
    }
}
    
void ParallelCoordinates3DAxis::draw_labels( const kvs::TableObject* table, const float dpr )
{
    const kvs::Vec3 min_coord = table->minObjectCoord();
    const kvs::Vec3 max_coord = table->maxObjectCoord();

    const kvs::Mat4 m = kvs::OpenGL::ModelViewMatrix();

    // Center of the boundary box in the object coordinate system.
    const kvs::Vec3 center = ( max_coord + min_coord ) * 0.5f;

    // Depth values (Z values) at each boundary plane in the camera coordinate system.
    const kvs::Real32 x_min = m[2].dot( kvs::Vec4( min_coord.x(), center.y(), center.z(), 1.0f ) );
    const kvs::Real32 x_max = m[2].dot( kvs::Vec4( max_coord.x(), center.y(), center.z(), 1.0f ) );
    const kvs::Real32 y_min = m[2].dot( kvs::Vec4( center.x(), min_coord.y(), center.z(), 1.0f ) );
    const kvs::Real32 y_max = m[2].dot( kvs::Vec4( center.x(), max_coord.y(), center.z(), 1.0f ) );
    const kvs::Real32 z_min = m[2].dot( kvs::Vec4( center.x(), center.y(), min_coord.z(), 1.0f ) );
    const kvs::Real32 z_max = m[2].dot( kvs::Vec4( center.x(), center.y(), max_coord.z(), 1.0f ) );

    kvs::Vector3f min_value = min_coord;
    kvs::Vector3f max_value = max_coord;
    kvs::Vector3f interval = max_value - min_value;
    interval /= static_cast<float>( m_number_of_grid + 1 );
    
    const kvs::Vector3f diff( max_coord - min_coord );
    const kvs::Real32 length = kvs::Math::Max( diff.x(), diff.y(), diff.z() );
    const kvs::Real32 label_offset = length * 0.1f;
    const kvs::Real32 value_offset = length * 0.05f;
    float min_y, max_y, min_z, max_z;
    for ( size_t i = 0; i < m_plane_positions.size(); i++ )
    {
        const float x_coord = m_plane_positions[i].x();
        min_y = table->minValue( 2 * i + 0 );
        max_y = table->maxValue( 2 * i + 0 );
        min_z = table->minValue( 2 * i + 1 );
        max_z = table->maxValue( 2 * i + 1 );
        float dvalue_y = max_y - min_y;
        dvalue_y /= static_cast<float>( m_number_of_grid + 1 );
        float dvalue_z = max_z - min_z;
        dvalue_z /= static_cast<float>( m_number_of_grid + 1 );
        // Draw Y label and values.
        {
            kvs::Vector3f position( x_coord, center.y(), min_coord.z() );
            //position.x() = x_min <= x_max ? min_coord.x() : max_coord.x();
            position.z() = z_min <= z_max ? max_coord.z() : min_coord.z();

            // Label.
            if ( m_show_labels )
            {
                kvs::Vector3f offset( -label_offset, 0.0f, -label_offset );
                offset.x() = x_min <= x_max ? -label_offset :  label_offset;
                offset.z() = z_min <= z_max ?  label_offset : -label_offset;
            
                kvs::OpenGL::Color( m_label_color.r(), m_label_color.g(), m_label_color.b() );
            
                const kvs::Vector3f label = position + offset;
                kvs::OpenGL::SetRasterPos( label.x(), label.y(), label.z() );
                char* head = const_cast<char*>( m_labels[ 2 * i + 0 ].c_str() );
                for ( char* p = head; *p; p++ ) { glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p ); }
            }
        
            // Values.
            if ( m_show_values )
            {
                kvs::Vector3f offset( -value_offset, 0.0f, -value_offset );
                offset.x() = x_min <= x_max ? -value_offset :  value_offset;
                offset.z() = z_min <= z_max ?  value_offset : -value_offset;
            
                kvs::OpenGL::Color( m_value_color.r(), m_value_color.g(), m_value_color.b() );
            
                const size_t nlines = size_t( m_number_of_grid ) + 2;
                for ( size_t i = 0; i < nlines; i++ )
                {
                    std::string number = kvs::String::ToString( min_y + dvalue_y * i );
                    const float x = position.x() + offset.x();
                    const float y = min_coord.y() + interval.y() * i;
                    const float z = position.z() + offset.y();
                    kvs::OpenGL::SetRasterPos( x, y, z );
                    char* head = const_cast<char*>( number.c_str() );
                    for ( char* p = head; *p; p++ ) { glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p ); }
                }
            }
        }   
    
        // Draw Z label and values.
        {
            kvs::Vector3f position( x_coord, min_coord.y(), center.z() );
            //position.x() = x_min <= x_max ? max_coord.x() : min_coord.x();
            position.y() = y_min <= y_max ? min_coord.y() : max_coord.y();
        
            // Label.
            if ( m_show_labels )
            {
                kvs::Vector3f offset( -label_offset, -label_offset, 0.0f );
                offset.x() = x_min <= x_max ?  label_offset : -label_offset;
                offset.y() = y_min <= y_max ? -label_offset :  label_offset;
            
                kvs::OpenGL::Color( m_label_color.r(), m_label_color.g(), m_label_color.b() );
            
                const kvs::Vector3f label = position + offset;
                kvs::OpenGL::SetRasterPos( label.x(), label.y(), label.z() );
                char* head = const_cast<char*>( m_labels[ 2 * i + 1 ].c_str() );
                for ( char* p = head; *p; p++ ) { glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p ); }
            }   
        
            // Values.
            if ( m_show_values )
            {
                kvs::Vector3f offset( -value_offset, -value_offset, 0.0f );
                offset.x() = x_min <= x_max ?  value_offset : -value_offset;
                offset.y() = y_min <= y_max ? -value_offset :  value_offset;
            
                kvs::OpenGL::Color( m_label_color.r(), m_value_color.g(), m_value_color.b() );
            
                const size_t nlines = size_t( m_number_of_grid ) + 2;
                for ( size_t i = 0; i < nlines; i++ )
                {
                    std::string number = kvs::String::ToString( min_z + dvalue_z * i );
                    const float x = position.x() + offset.x();
                    const float y = position.y() + offset.y();
                    const float z = min_coord.z() + interval.z() * i;
                    kvs::OpenGL::SetRasterPos( x, y, z );
                    char* head = const_cast<char*>( number.c_str() );
                    for( char* p = head; *p; p++ ) { glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p ); }
                }   
            }
        }
    }
}

} // end of namespace local
