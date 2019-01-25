#pragma once
#include "EventListener.h"
#include <Lib/Screen.h>


namespace kvs
{

namespace oculus
{

class TouchEventListener : public kvs::oculus::EventListener
{
private:
    bool m_is_grabbed;
    bool m_both_is_grabbed;
    float m_touch_distance;

public:
    TouchEventListener( kvs::oculus::Screen* screen );

    virtual void initializeEvent();
    virtual void frameEvent();
};

} // end of namespace oculus

} // end of namespace kvs