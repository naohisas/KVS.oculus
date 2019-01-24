#pragma once
#include <kvs/glut/Screen>
#include <kvs/FrameBufferObject>
#include "Application.h"
#include "HeadMountedDisplay.h"


namespace kvs
{

namespace oculus
{

class Screen : public kvs::glut::Screen
{
private:
    kvs::oculus::HeadMountedDisplay m_hmd; ///< Oculus HMD

public:
    Screen( kvs::oculus::Application* app = 0 );
    virtual ~Screen();

    const kvs::oculus::HeadMountedDisplay& headMountedDisplay() const { return m_hmd; }

    virtual void show( const bool fullscreen = true );
    virtual void initializeEvent();
    virtual void paintEvent();
};

} // end of namespace oculus

} // end of namespace kvs
