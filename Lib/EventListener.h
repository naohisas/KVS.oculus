#pragma once
#include <kvs/EventListener>
#include "Screen.h"
#include "InputDevice.h"


namespace kvs
{

namespace oculus
{

class EventListener : public kvs::EventListener
{
private:
    kvs::oculus::InputDevice m_input_device; ///< input device

public:
    EventListener( kvs::oculus::Screen* screen );

    virtual void initializeEvent() {}
    virtual void frameEvent() {}

protected:
    kvs::oculus::InputDevice& inputDevice() { return m_input_device; }

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace oculus

} // end of namespace kvs
