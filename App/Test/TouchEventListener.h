#pragma once
#include "EventListener.h"
#include <Lib/Screen.h>


namespace kvs
{

namespace oculus
{

class TouchEventListener : public kvs::oculus::EventListener
{
public:
    TouchEventListener( kvs::oculus::Screen* screen );

    virtual void initializeEvent();
    virtual void frameEvent();
};

} // end of namespace oculus

} // end of namespace kvs