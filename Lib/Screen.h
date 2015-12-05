#pragma once
#include <kvs/glut/ScreenBase>
#include <kvs/FrameBufferObject>
#include "HeadMountedDisplay.h"


namespace kvs
{

namespace oculus
{

class Screen : public kvs::glut::ScreenBase
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
    Screen();
    virtual ~Screen();

    virtual void initializeEvent();
    virtual void paintEvent();
    virtual void resizeEvent( int width, int height );
    virtual void mousePressEvent( kvs::MouseEvent* event );
    virtual void mouseMoveEvent( kvs::MouseEvent* event );
    virtual void mouseReleaseEvent( kvs::MouseEvent* event );
    virtual void mouseDoubleClickEvent( kvs::MouseEvent* event );
    virtual void wheelEvent( kvs::WheelEvent* event );
    virtual void keyPressEvent( kvs::KeyEvent* event );
    virtual void idleMouseEvent();
};

} // end of namespace oculus

} // end of namespace kvs
