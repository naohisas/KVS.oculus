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
    kvs::oculus::HeadMountedDisplay m_hmd;
    kvs::FrameBufferObject m_framebuffer;
    kvs::Texture2D m_color_texture;
    kvs::RenderBuffer m_depth_renderbuffer;

    ovrRecti m_viewport[2];
    ovrGLTexture m_texture[2];
    ovrEyeRenderDesc m_desc[2];

public:
    Screen( kvs::oculus::Application* app = 0 );
    virtual ~Screen();

    virtual void initializeEvent();
    virtual void defaultPaintEvent();
    virtual void defaultResizeEvent( int width, int height );
};

} // end of namespace oculus

} // end of namespace kvs
