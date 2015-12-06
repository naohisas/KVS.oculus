#include "Screen.h"
#include <kvs/Platform>


namespace kvs
{

namespace oculus
{

Screen::Screen( kvs::oculus::Application* app ) : kvs::glut::Screen( app )
{
    static bool flag = true;
    if ( flag )
    {
        flag = false;
        m_hmd.create();
        if ( !m_hmd.handler() ) { m_hmd.createDebug( ovrHmd_DK2 ); }
    }
}

Screen::~Screen()
{
   m_hmd.destroy();
}

void Screen::initializeEvent()
{
    kvs::glut::Screen::initializeEvent();

    const ovrEyeType eye0 = ovrEye_Left;
    const ovrFovPort fov0 = m_hmd.defaultEyeFov(0);
    const ovrSizei tex0 = m_hmd.fovTextureSize( eye0, fov0, 1.0f );

    const ovrEyeType eye1 = ovrEye_Right;
    const ovrFovPort fov1 = m_hmd.defaultEyeFov(1);
    const ovrSizei tex1 = m_hmd.fovTextureSize( eye1, fov1, 1.0f );

    ovrSizei target_size;
    target_size.w = tex0.w + tex1.w;
    target_size.h = kvs::Math::Max( tex0.h, tex1.h );

    m_color_texture.setMagFilter( GL_LINEAR );
    m_color_texture.setMinFilter( GL_LINEAR );
    m_color_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_color_texture.setWrapT( GL_CLAMP_TO_EDGE );
    m_color_texture.setPixelFormat( GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE );
    m_color_texture.create( target_size.w, target_size.h );

    m_depth_renderbuffer.setInternalFormat( GL_DEPTH_COMPONENT24 );
    m_depth_renderbuffer.create( target_size.w, target_size.h );

    m_framebuffer.create();
    m_framebuffer.bind();
    m_framebuffer.attachColorTexture( m_color_texture );
    m_framebuffer.attachDepthRenderBuffer( m_depth_renderbuffer );
    m_framebuffer.unbind();

    m_viewport[0].Pos = OVR::Vector2i( 0, 0 );
    m_viewport[0].Size = OVR::Sizei( target_size.w / 2, target_size.h );
    m_viewport[1].Pos = OVR::Vector2i( ( target_size.w + 1 ) / 2, 0 );
    m_viewport[1].Size = m_viewport[0].Size;

    m_texture[0].OGL.Header.API = ovrRenderAPI_OpenGL;
    m_texture[0].OGL.Header.TextureSize = target_size;
    m_texture[0].OGL.Header.RenderViewport = m_viewport[0];
    m_texture[0].OGL.TexId = m_color_texture.id();
    m_texture[1] = m_texture[0];
    m_texture[1].OGL.Header.RenderViewport = m_viewport[1];

    ovrGLConfig config;
    config.OGL.Header.API = ovrRenderAPI_OpenGL;
    config.OGL.Header.BackBufferSize = OVR::Sizei( m_hmd.resolution().w, m_hmd.resolution().h );
    config.OGL.Header.Multisample = 1;

#if defined( KVS_PLATFORM_WINDOWS )
    HDC dc = wglGetCurrentDC();
    HWND wnd = WindowFromDC( dc );

    if ( !( m_hmd.hmdCaps() & ovrHmdCap_ExtendDesktop ) )
    {
        ovrHmd_AttachToWindow( m_hmd, wnd, NULL, NULL );
    }

    config.OGL.Window = wnd;
    config.OGL.DC = dc;
#endif

    kvs::UInt32 caps = ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction;
    m_hmd.setEnabledCaps( caps );

    ovrFovPort fov[2] = { fov0, fov1 };
    kvs::UInt32 dist_caps = ovrDistortionCap_Vignette | ovrDistortionCap_TimeWarp | ovrDistortionCap_Overdrive;
    if ( !m_hmd.configureRendering( &config.Config, dist_caps, fov, m_desc ) )
    {
        kvsMessageError("Failed rendering configuration.");
    }

    kvs::UInt32 trac_caps = ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position;
    if ( !m_hmd.configureTracking( trac_caps, 0 ) )
    {
        kvsMessageError("Failed tracking configuration");
    }
}

void Screen::defaultPaintEvent()
{
//    kvs::glut::Screen::defaultPaintEvent();
}

void Screen::defaultResizeEvent( int width, int height )
{
//    kvs::glut::Screen::defaultResizeEvent( width, height );
}

} // end of namespace oculus

} // end of namespace kvs
