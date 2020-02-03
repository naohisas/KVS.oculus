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
    bool m_is_grabbed;
    bool m_both_is_grabbed;
    float m_touch_distance;
    float m_scaling_factor;
    float m_change_factor;
    float m_size_factor;
    float m_left_x;
    float m_right_x;

public:
    MyTouchController( kvs::oculus::Screen* screen );
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
        MyTouchController( kvs::glut::Screen* screen );
        
        void keyPressEvent( kvs::KeyEvent* event );
        
        void mousePressEvent( kvs::MouseEvent* event )
        {
            std::cout << "mouse presses." << std::endl;
        }
    };
}
#endif
