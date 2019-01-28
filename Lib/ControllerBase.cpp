#include "ControllerBase.h"


namespace kvs
{

namespace oculus
{

ControllerBase::ControllerBase( kvs::oculus::Screen* screen ):
    m_input_device( screen->headMountedDisplay() )
{
    setScreen( screen );
    setScene( screen->scene() );
    setEventType( kvs::EventBase::InitializeEvent | kvs::EventBase::PaintEvent );
    screen->addEvent( this );
}

void ControllerBase::onEvent( kvs::EventBase* event )
{
    switch ( event->type() )
    {
    case kvs::EventBase::InitializeEvent:
    {
        m_input_device.initialize();
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