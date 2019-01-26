#include "EventListener.h"


namespace kvs
{

namespace oculus
{

EventListener::EventListener( kvs::oculus::Screen* screen ):
    m_controller( screen->headMountedDisplay() )
{
    setScreen( screen );
    setScene( screen->scene() );
    setEventType( kvs::EventBase::InitializeEvent | kvs::EventBase::PaintEvent );
    screen->addEvent( this );
}

void EventListener::onEvent( kvs::EventBase* event )
{
    switch ( event->type() )
    {
    case kvs::EventBase::InitializeEvent:
    {
        m_controller.initialize();
        this->initializeEvent();
        break;
    }
    case kvs::EventBase::PaintEvent:
    {
        this->frameEvent();
        break;
    }
    default: break;
    }
}

} // end of namespace oculus

} // end of namespace kvs