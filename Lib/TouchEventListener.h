#pragma once
#include "EventListener.h"
#include "Screen.h"


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
    float m_rotation_factor;
    float m_translation_factor;
    float m_scaling_factor;

public:
    TouchEventListener( kvs::oculus::Screen* screen );

    float rotationFactor() const { return m_rotation_factor; }
    float translationFactor() const { return m_translation_factor; }
    float scalingFactor() const { return m_scaling_factor; }
    void setRotationFactor( const float factor ) { m_rotation_factor = factor; }
    void setTranslationFactor( const float factor ) { m_translation_factor = factor; }
    void setScalingFactor( const float factor ) { m_scaling_factor = factor; }

    virtual void initializeEvent();
    virtual void frameEvent();
};

} // end of namespace oculus

} // end of namespace kvs
