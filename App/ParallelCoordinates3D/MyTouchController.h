#pragma once
#include "TouchController.h"
#if defined( KVS_SUPPORT_OCULUS )
#include <Lib/OVR.h>
#include "BundledParallelCoordinates3DRenderer.h"

namespace local
{

class MyTouchController : public local::TouchController
{
public:
    MyTouchController( kvs::oculus::Screen* screen ):local::TouchController( screen ) {};
    void frameEvent()
    {
        local::TouchController::frameEvent();
        const ovrTrackingState ts = inputDevice().trackingState( 0 );
        ovrPosef hands[2] = { ts.HandPoses[ovrHand_Left].ThePose, ts.HandPoses[ovrHand_Right].ThePose };
        
        ovrInputState input_state = inputDevice().inputState( ovrControllerType_Touch );

        if ( input_state.Buttons & ovrButton_A )
        {
            std::cout << "ReducedPlaneScale mode" << std::endl;

            typedef local::BundledParallelCoordinates3DRenderer Renderer;
            Renderer* renderer = Renderer::DownCast( scene()->renderer("Renderer") );
            float scale = renderer->m_reduced_plane_scale; 
            scale = scale + 0.1f;
            renderer->setReducedPlaneScale( scale ); 
        }
        if ( input_state.Buttons & ovrButton_B )
        {
            std::cout << "ReducedPlaneScale mode" << std::endl;

            typedef local::BundledParallelCoordinates3DRenderer Renderer;
            Renderer* renderer = Renderer::DownCast( scene()->renderer("Renderer") );
            float scale = renderer->m_reduced_plane_scale; 
            scale = scale - 0.1f;
            if (scale < 0.1)
            {
                scale = 0.1;
            }
            renderer->setReducedPlaneScale( scale ); 
        }
        if ( input_state.Buttons & ovrButton_X )
        {
            std::cout << "CurveSizeScale mode" << std::endl;

            typedef local::BundledParallelCoordinates3DRenderer Renderer;
            Renderer* renderer = Renderer::DownCast( scene()->renderer("Renderer") );
            float scale = renderer->m_curve_size; 
            scale = scale + 0.01f;
            if (scale > 1.0)
            {
                scale = 1.0;
            }
            renderer->setCurveSize( scale ); 
        }
        if ( input_state.Buttons & ovrButton_Y )
        {
            std::cout << "CurveSizeScale mode" << std::endl;

            typedef local::BundledParallelCoordinates3DRenderer Renderer;
            Renderer* renderer = Renderer::DownCast( scene()->renderer("Renderer") );
            float scale = renderer->m_curve_size; 
            scale = scale - 0.01f;
            if (scale < 0.01)
            {
                scale = 0.01f;
            }
            renderer->setCurveSize( scale ); 
        }
        ovrVector2f a = input_state.Thumbstick[ovrHand_Left];
        //std::cout << "x=" << a.x << std::endl;
        //std::cout << "y=" << a.y << std::endl;
        if (a.x > 0.5)
        {
            typedef local::BundledParallelCoordinates3DRenderer Renderer;
            Renderer* renderer = Renderer::DownCast( scene()->renderer("Renderer") );
            size_t p = renderer->bundledPosition();
            std::cout << "p = " << p << std::endl;
            kvs::TableObject* object = kvs::TableObject::DownCast( scene()->object("Object") );
            size_t n = object->numberOfColumns() / 2;
            std::cout << "n = " << n << std::endl;
            p = p == n - 2 ? p : p + 1;
            renderer->setBundledPosition(p);
        }
        else if (a.x < -0.5)
        {
            typedef local::BundledParallelCoordinates3DRenderer Renderer;
            Renderer* renderer = Renderer::DownCast( scene()->renderer("Renderer") );
            size_t p = renderer->bundledPosition();
            kvs::TableObject* object = kvs::TableObject::DownCast( scene()->object("Object") );
            size_t n = object->numberOfColumns();
            p = p == 0 ? p : p - 1;
            renderer->setBundledPosition(p);
        }
    }
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
            switch ( event->key() )
            {
                case kvs::Key::a:
                    std::cout << "a" << std::endl;
                    break;
                case kvs::Key::One:
                    std::cout << "1" << std::endl;
                    break;
                case kvs::Key::Left:
                    std::cout << "Left" << std::endl;
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
