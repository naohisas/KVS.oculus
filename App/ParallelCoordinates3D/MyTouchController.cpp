#include "MyTouchController.h"
#if defined( KVS_SUPPORT_OCULUS )

namespace local
{
MyTouchController::MyTouchController( kvs::oculus::Screen* screen ):
        local::TouchController( screen ),
        m_is_grabbed( false ),
        m_both_is_grabbed( false ),
        m_touch_distance( 0.0f ),
        m_scaling_factor( 10.0f ),
        m_change_factor( 0.0f ),
        m_size_factor( 10.0 ),
        m_left_x( 0.0f ),
        m_right_x( 0.0f )
{
    
}

void MyTouchController::frameEvent()
{
    local::TouchController::frameEvent();
    const ovrTrackingState ts = inputDevice().trackingState( 0 );
    ovrPosef hands[2] = { ts.HandPoses[ovrHand_Left].ThePose, ts.HandPoses[ovrHand_Right].ThePose };
    
    ovrInputState input_state = inputDevice().inputState( ovrControllerType_Touch );
        
    const kvs::Vec3 right_p = kvs::oculus::ToVec3( hands[ovrHand_Right].Position );
    //const kvs::Vec2i right_m( kvs::Vec2( right_p.x(), -right_p.y() ) * m_rotation_factor );

    const kvs::Vec3 left_p = kvs::oculus::ToVec3( hands[ovrHand_Left].Position );
    //const kvs::Vec2i left_m( kvs::Vec2( left_p.x(), -left_p.y() ) * m_size_factor );

    const bool right_grabbed = input_state.IndexTrigger[ovrHand_Right] > 0.5f;
    const bool left_grabbed = input_state.IndexTrigger[ovrHand_Left] > 0.5f;
    ovrVector2f a = input_state.Thumbstick[ovrHand_Left];
    //std::cout << "x=" << a.x << std::endl;
    //std::cout << "y=" << a.y << std::endl;

    typedef local::BundledParallelCoordinates3DRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( scene()->renderer("Renderer") );

    float scale, size;
    size_t p, n;
    kvs::TableObject* object = kvs::TableObject::DownCast( scene()->object("Object") );
    n = object->numberOfColumns() / 2;

    if ( right_grabbed && left_grabbed )
    {
        if ( m_both_is_grabbed )
        {
            const double touch_distance = ( right_p - left_p ).length();
            const double d = touch_distance - m_touch_distance;
            scale = renderer->m_reduced_plane_scale + d * m_scaling_factor;
            if (scale < 0)
            {
            scale = 0;
            }
            renderer->setReducedPlaneScale( scale );
            m_touch_distance = static_cast<float>( touch_distance );
            //std::cout << "d = " << d << std::endl;
        }
        else
        {
            m_both_is_grabbed = true;
            m_touch_distance = static_cast<float>( ( right_p - left_p ).length() );
            m_is_grabbed = false;
            /*
            if ( scene()->mouse()->operationMode() == kvs::Mouse::Rotation )
            {
                scene()->mouseReleaseFunction( right_m.x(), right_m.y() );
            }
            else if ( scene()->mouse()->operationMode() == kvs::Mouse::Translation )
            {
                scene()->mouseReleaseFunction( left_m.x(), left_m.y() );
            }*/
        }
    }
    else if ( input_state.Buttons & ovrButton_A )
    {
        renderer->setReducedPlaneScale( 0.0f ); 
    }
    else if ( input_state.Buttons & ovrButton_B )
    {
        renderer->setReducedPlaneScale( 1.0f ); 
    }
    else if ( left_grabbed && !right_grabbed )
    {
        if ( m_is_grabbed )
        {
            const double left_x = left_p.x();
            const double d = left_x - m_left_x;
            size = renderer->m_curve_size + d * m_size_factor;
            if (size < 0)
            {
                size = 0.0f;
            }
            else if (size > 0.9)
            {
                size = 0.9f;
            }
            renderer->setCurveSize( size );
            m_left_x = static_cast<float>( left_x );
            //std::cout << "d = " << d << std::endl;
        }
        else
        {
            m_is_grabbed = true;
            m_left_x = static_cast<float>( left_p.x() );
            m_both_is_grabbed = false;
        }
    }
    else if ( right_grabbed && !left_grabbed )
    {
        if ( m_is_grabbed )
        {
            const double right_x = right_p.x();
            const double d = m_right_x - right_x;
            size = renderer->m_curve_size + d * m_size_factor;
            if (size < 0)
            {
                size = 0.0f;
            }
            else if (size > 0.9)
            {
                size = 0.9f;
            }
            renderer->setCurveSize( size );
            m_right_x = static_cast<float>( right_x );
            //std::cout << "d = " << d << std::endl;
        }
        else
        {
            m_is_grabbed = true;
            m_right_x = static_cast<float>( right_p.x() );
            m_both_is_grabbed = false;
        }
    }
    else if ( input_state.Buttons & ovrButton_X )
    {
        renderer->setCurveSize( 0.0f );
    }
    else if ( input_state.Buttons & ovrButton_Y )
    {
        renderer->setCurveSize( 0.5f );
    }
    else if (a.x > 0.5)
    {
        m_change_factor = m_change_factor + 0.1f;
        //std::cout << "m_change_factor = " << m_change_factor << std::endl;
        if( m_change_factor > 1.5f )
        {
            p = renderer->bundledPosition();
            p = p == n - 2 ? p : p + 1;
            renderer->setBundledPosition(p);
            m_change_factor = 0.0f;
        }
    }
    else if (a.x < -0.5)
    {
        m_change_factor = m_change_factor - 0.1f;
        //std::cout << "m_change_factor = " << m_change_factor << std::endl;
        if( m_change_factor < -1.5f )
        {
            p = renderer->bundledPosition();
            p = p == 0 ? p : p - 1;
            renderer->setBundledPosition(p);
            m_change_factor = 0.0f;
        }
    }
    else
    {
        m_is_grabbed = false;
        m_both_is_grabbed = false;
        m_touch_distance = 0.0f;
        m_change_factor = 0.0f;
    }
}
}

#else

namespace local
{
    MyTouchController::MyTouchController( kvs::glut::Screen* screen )
    {
        setEventType( kvs::EventBase::KeyPressEvent | kvs::EventBase::MousePressEvent );
        screen->addEvent( this );
    }
    
    void MyTouchController::keyPressEvent( kvs::KeyEvent* event )
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
            case kvs::Key::Right:
                std::cout << "CurveSizeScale mode" << std::endl;
                size = size - 0.1f;
                if (size < 0 )
                {
                    size = 0.0f;
                }
                std::cout << "Curve_Size = " << size << std::endl;
                renderer->setCurveSize( size );
                break;
            case kvs::Key::Left:
                std::cout << "CurveSizeScale mode" << std::endl;
                size = size + 0.1f;
                if (size > 0.9)
                {
                    size = 0.9f;
                }
                std::cout << "Curve_Size = " << size << std::endl;
                renderer->setCurveSize( size );
                break;
            case kvs::Key::a:
                std::cout << "CurveSizeScale mode" << std::endl;
                size = size - 0.1f;
                if (size < 0 )
                {
                    size = 0.0f;
                }
                std::cout << "Curve_Size = " << size << std::endl;
                renderer->setCurveSize( size );
                break;
            case kvs::Key::d:
                std::cout << "CurveSizeScale mode" << std::endl;
                size = size + 0.1f;
                if (size > 0.9)
                {
                    size = 0.9f;
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
            case kvs::Key::Up:
                std::cout << "ReducedPlaneScale mode" << std::endl;
                scale = scale + 0.1f;
                std::cout << "Reduced_Plane_Size = " << scale << std::endl;
                renderer->setReducedPlaneScale( scale );
                break;
            case kvs::Key::Down:
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
}
#endif
