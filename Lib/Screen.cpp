#include "Screen.h"
#include <kvs/OpenGL>
#include <kvs/Platform>
#include <kvs/Background>
#include <kvs/Camera>
#include <kvs/ObjectManager>
#include <kvs/RendererManager>
#include <kvs/IDManager>
#include <kvs/RendererBase>


namespace
{

inline kvs::Vec3 ToVec3( const OVR::Vector3f& v )
{
    return kvs::Vec3( v.x, v.y, v.z );
}

inline OVR::Vector3f ToVec3( const kvs::Vec3& v )
{
    return OVR::Vector3f( v.x(), v.y(), v.z() );
}

inline kvs::Mat4 ToMat4( const OVR::Matrix4f& m )
{
    return kvs::Mat4(
        m.M[0][0], m.M[0][1], m.M[0][2], m.M[0][3],
        m.M[1][0], m.M[1][1], m.M[1][2], m.M[1][3],
        m.M[2][0], m.M[2][1], m.M[2][2], m.M[2][3],
        m.M[3][0], m.M[3][1], m.M[3][2], m.M[3][3] );
}

inline OVR::Matrix4f ToMat4( const kvs::Mat3& m )
{
    return OVR::Matrix4f(
        m[0][0], m[0][1], m[0][2],
        m[1][0], m[1][1], m[1][2],
        m[2][0], m[2][1], m[2][2] );
}

inline OVR::Matrix4f ToMat4( const kvs::Mat4& m )
{
    return OVR::Matrix4f(
        m[0][0], m[0][1], m[0][2], m[0][3],
        m[1][0], m[1][1], m[1][2], m[1][3],
        m[2][0], m[2][1], m[2][2], m[2][3],
        m[3][0], m[3][1], m[3][2], m[3][3] );
}

}

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
        if ( !m_hmd.create() ) { m_hmd.createDebug( ovrHmd_DK1 ); }
        setSize( m_hmd.resolution().w, m_hmd.resolution().h );
    }

    scene()->camera()->setProjectionTypeToFrustum();
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

//    std::cout << "fov0.UpTan = " << fov0.UpTan << std::endl;
//    std::cout << "fov0.DownTan = " << fov0.DownTan << std::endl;
//    std::cout << "fov0.LeftTan = " << fov0.LeftTan << std::endl;
//    std::cout << "fov0.RightTan = " << fov0.RightTan << std::endl;
//    std::cout << std::endl;
//    std::cout << "tex0.w = " << tex0.w << std::endl;
//    std::cout << "tex0.h = " << tex0.h << std::endl;
//    std::cout << std::endl;

    const ovrEyeType eye1 = ovrEye_Right;
    const ovrFovPort fov1 = m_hmd.defaultEyeFov(1);
    const ovrSizei tex1 = m_hmd.fovTextureSize( eye1, fov1, 1.0f );

//    std::cout << "fov1.UpTan = " << fov1.UpTan << std::endl;
//    std::cout << "fov1.DownTan = " << fov1.DownTan << std::endl;
//    std::cout << "fov1.LeftTan = " << fov1.LeftTan << std::endl;
//    std::cout << "fov1.RightTan = " << fov1.RightTan << std::endl;
//    std::cout << std::endl;
//    std::cout << "tex1.w = " << tex1.w << std::endl;
//    std::cout << "tex1.h = " << tex1.h << std::endl;
//    std::cout << std::endl;

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

void Screen::paintEvent()
{
    ovrTrackingState ts = m_hmd.trackingState( ovr_GetTimeInSeconds() );

    kvs::OpenGL::WithPushedMatrix p( GL_MODELVIEW );
    p.loadIdentity();
    {
        m_hmd.beginFrame( 0 );
        m_framebuffer.bind();

        scene()->background()->apply();
        scene()->updateGLLightParameters();

        const kvs::Vec3 camera_position = scene()->camera()->position();
        const kvs::Vec3 camera_lookat = scene()->camera()->lookAt();
        const kvs::Vec3 camera_upvector = scene()->camera()->upVector();

        const OVR::Vector3f position0 = ::ToVec3( camera_position );
        const OVR::Vector3f lookat0 = ::ToVec3( camera_lookat );
        const OVR::Vector3f upvector0 = ::ToVec3( camera_upvector );
        const OVR::Matrix4f rotation0 = ::ToMat4( scene()->camera()->xform().rotation() );

        ovrPosef eye_pose[2];
        const size_t neyes = ovrEye_Count;
        for ( size_t i = 0; i < neyes; i++ )
        {
//            std::cout << "eye_pose[" << i << "]" << std::endl;
//            std::cout << std::endl;

            const ovrEyeType eye = m_hmd.eyeRenderOrder( i );
            eye_pose[i] = m_hmd.posePerEye( eye );

            // Setup the viewport.
            const ovrVector2i pos = m_viewport[i].Pos;
            const ovrSizei size = m_viewport[i].Size;
            kvs::OpenGL::SetViewport( pos.x, pos.y, size.w, size.h );

//            std::cout << "pos.x = " << pos.x << std::endl;
//            std::cout << "pos.y = " << pos.y << std::endl;
//            std::cout << "size.w = " << size.w << std::endl;
//            std::cout << "size.h = " << size.h << std::endl;
//            std::cout << std::endl;

            // Setup the projection matrix.
            const float front = scene()->camera()->front();

//            std::cout << "front = " << front << std::endl;
//            std::cout << "Fov.UpTan = " << m_desc[i].Fov.UpTan << std::endl;
//            std::cout << "Fov.DownTan = " << -m_desc[i].Fov.DownTan << std::endl;
//            std::cout << "Fov.LeftTan = " << -m_desc[i].Fov.LeftTan << std::endl;
//            std::cout << "Fov.RightTan = " << m_desc[i].Fov.RightTan << std::endl;

            scene()->camera()->setTop( m_desc[i].Fov.UpTan * front );
            scene()->camera()->setBottom( -m_desc[i].Fov.DownTan * front );
            scene()->camera()->setLeft( -m_desc[i].Fov.LeftTan * front );
            scene()->camera()->setRight( m_desc[i].Fov.RightTan * front );
            scene()->updateGLProjectionMatrix();

            // Setup the viewing matrix.
            OVR::Matrix4f R0 = rotation0;
            OVR::Matrix4f R = R0 * OVR::Matrix4f( ts.HeadPose.ThePose.Orientation );
            OVR::Vector3f T = R0.Transform( ts.HeadPose.ThePose.Position );
            OVR::Vector3f upvector = R.Transform( upvector0 );
            OVR::Vector3f forward = R.Transform( lookat0 - position0 );
            OVR::Vector3f position = position0 + T + R.Transform( eye_pose[i].Position );
            OVR::Vector3f lookat = position + forward;
            scene()->camera()->setPosition( ::ToVec3( position ), ::ToVec3( lookat ), ::ToVec3( upvector ) );
            scene()->updateGLViewingMatrix();

            // Setup the modeling matrix and render the objects.
            if ( scene()->objectManager()->hasObject() )
            {
                const int size = scene()->IDManager()->size();
                for ( int index = 0; index < size; index++ )
                {
                    kvs::IDManager::IDPair id = scene()->IDManager()->id( index );
                    kvs::ObjectBase* object = scene()->objectManager()->object( id.first );
                    kvs::RendererBase* renderer = scene()->rendererManager()->renderer( id.second );
                    if ( object->isShown() )
                    {
                        kvs::OpenGL::PushMatrix();
                        scene()->updateGLModelingMatrix( object );
                        renderer->exec( object, scene()->camera(), scene()->light() );
                        kvs::OpenGL::PopMatrix();
                    }
                }
            }
            else
            {
                scene()->updateGLModelingMatrix();
            }
        }

        scene()->camera()->setPosition( camera_position, camera_lookat, camera_upvector );

        m_framebuffer.unbind();
        m_hmd.endFrame( eye_pose, &m_texture[0].Texture );
    }
    kvs::OpenGL::Flush();
}

} // end of namespace oculus

} // end of namespace kvs
