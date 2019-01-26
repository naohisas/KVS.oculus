#pragma once
#include <kvs/EventListener>
#include "Screen.h"
#include "Controller.h"


namespace kvs
{

namespace oculus
{

class EventListener : public kvs::EventListener
{
private:
    kvs::oculus::Controller m_controller; ///< controller

public:
    EventListener( kvs::oculus::Screen* screen );

    virtual void initializeEvent() {}
    virtual void frameEvent() {}

protected:
    kvs::oculus::Controller& controller() { return m_controller; }

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace oculus

} // end of namespace kvs
