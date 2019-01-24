#include "TouchEventListener.h"

namespace kvs
{

namespace oculus
{

TouchEventListener::TouchEventListener( kvs::oculus::Screen* screen ):
    kvs::oculus::EventListener( screen )
{
}

void TouchEventListener::initializeEvent()
{
    std::cout << "TouchEventListener::initializeEvent()" << std::endl;
}

void TouchEventListener::frameEvent()
{
    std::cout << "TouchEventListener::frameEvent()" << std::endl;
}

} // end of namespace oculus

} // end of namespace kvs