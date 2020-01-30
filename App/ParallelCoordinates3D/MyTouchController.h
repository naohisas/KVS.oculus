#pragma once
#include "TouchController.h"
#include "BundledParallelCoordinates3DRenderer.h"
#if defined( KVS_SUPPORT_OCULUS )
#include <Lib/OVR.h>

namespace local
{

class MyTouchController : public local::TouchController
{
private:
    bool m_both_is_pushed;
    float m_touch_distance;
    float m_scaling_factor;

public:
    MyTouchController( kvs::oculus::Screen* screen ):
        local::TouchController( screen ),
        m_both_is_pushed( false ),
        m_touch_distance( 0.0f ),
        m_scaling_factor( 30.0f ) {};
    void frameEvent();
};

}

#else
//namespace local { typedef TouchController MyTouchController; }
#include <kvs/EventListener>

namespace local
{
    class MyTouchController : public kvs::EventListener
    {
    public:
        MyTouchController( kvs::glut::Screen* screen )
        {
            setEventType( kvs::EventBase::KeyPressEvent | kvs::EventBase::MousePressEvent );
            screen->addEvent( this );
        }
        
        void keyPressEvent( kvs::KeyEvent* event )
        {
            typedef local::BundledParallelCoordinates3DRenderer Renderer;
            Renderer* renderer = Renderer::DownCast( scene()->renderer("Renderer") );
            float size = renderer->m_curve_size;
            float scale = renderer->m_reduced_plane_scale;
            kvs::TableObject* object = kvs::TableObject::DownCast( scene()->object("Object") );
            size_t numberOfPlanes = object->numberOfColumns() / 2 - 2;
            size_t position = renderer->m_bundled_position;
            switch ( event->key() )
            {
                case kvs::Key::z:
                    std::cout << "BundledPosition mode" << std::endl;
                    position = position - 1;
                    if (position > numberOfPlanes )
                    {
                        position = 0;
                    }
                    std::cout << "Bundled_Position = " << position << std::endl;
                    renderer->setBundledPosition( position );
                    break;
                case kvs::Key::c:
                    std::cout << "BundledPosition mode" << std::endl;
                    position = position + 1;
                    if (position > numberOfPlanes )
                    {
                        position = numberOfPlanes;
                    }
                    std::cout << "Bundled_Position = " << position << std::endl;
                    renderer->setBundledPosition( position );
                    break;
                case kvs::Key::Down:
                    std::cout << "CurveSizeScale mode" << std::endl;
                    size = size - 0.1f;
                    if (size < 0 )
                    {
                        size = 0.0f;
                    }
                    std::cout << "Curve_Size = " << size << std::endl;
                    renderer->setCurveSize( size );
                    break;
                case kvs::Key::Up:
                    std::cout << "CurveSizeScale mode" << std::endl;
                    size = size + 0.1f;
                    if (size > 1.0)
                    {
                        size = 1.0f;
                    }
                    std::cout << "Curve_Size = " << size << std::endl;
                    renderer->setCurveSize( size );
                    break;
                case kvs::Key::s:
                    std::cout << "ReducedPlaneScale mode" << std::endl;
                    scale = scale + 0.1f;
                    std::cout << "Reduced_Plane_Size = " << scale << std::endl;
                    renderer->setReducedPlaneScale( scale );
                    break;
                case kvs::Key::x:
                    std::cout << "ReducedPlaneScale mode" << std::endl;
                    scale = scale - 0.1f;
                    if (scale < 0 )
                    {
                        scale = 0.0f;
                    }
                    std::cout << "Reduced_Plane_Size = " << scale << std::endl;
                    renderer->setReducedPlaneScale( scale );
                    break;
                default:
                    break;
            }
        }
        
        void mousePressEvent( kvs::MouseEvent* event )
        {
            std::cout << "mouse presses." << std::endl;
        }
    };
}
#endif
