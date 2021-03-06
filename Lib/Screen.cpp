#include "Screen.h"
#include <kvs/OpenGL>
#include <kvs/Platform>
#include <kvs/Background>
#include <kvs/Camera>
#include <kvs/ObjectManager>
#include <kvs/RendererManager>
#include <kvs/IDManager>
#include <kvs/RendererBase>
#include <kvs/PaintEvent>
#include <kvs/glut/GLUT>


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

Screen::Screen( kvs::oculus::Application* app ):
    kvs::glut::Screen( app ),
    m_mirror_buffer_type( DistortedBothEyeImage )
{
    static bool flag = true;
    if ( flag )
    {
        flag = false;
        if ( m_hmd.create() )
        {
            const int desktop_width = glutGet( GLUT_SCREEN_WIDTH );
            const int desktop_height = glutGet( GLUT_SCREEN_HEIGHT );

            const int hmd_width = m_hmd.resolution().w;
            const int hmd_height = m_hmd.resolution().h;

            const float ratio = static_cast<float>( desktop_width ) / hmd_width;
            const int screen_width = ( ratio >= 1.0f ) ? hmd_width : desktop_width;
            const int screen_height = ( ratio >= 1.0f ) ? hmd_height : static_cast<int>( hmd_height * ratio );
            setSize( screen_width, screen_height );
        }
    }

    scene()->camera()->setProjectionTypeToFrustum();
}

Screen::~Screen()
{
   m_hmd.destroy();
}

void Screen::show( const bool fullscreen )
{
    kvs::glut::Screen::show();
    if ( fullscreen ) { kvs::glut::Screen::showFullScreen(); }
}

void Screen::initializeEvent()
{
    // Configure tracking.
    const kvs::UInt32 trac_caps =
        ovrTrackingCap_Orientation |
        ovrTrackingCap_MagYawCorrection |
        ovrTrackingCap_Position;
    if ( !m_hmd.configureTracking( trac_caps, 0 ) )
    {
        kvsMessageError("Failed tracking configuration");
    }

    // Configure rendering.
    if ( !m_hmd.configureRendering() )
    {
        kvsMessageError("Failed rendering configuration.");
    }

    kvs::glut::Screen::initializeEvent();
}

void Screen::paintEvent()
{
    const kvs::Int64 frame_index = 0;
    const double frame_timing = m_hmd.frameTiming( frame_index );
    const ovrTrackingState& ts = m_hmd.trackingState( frame_timing );
    const kvs::Vec4 current_viewport = kvs::OpenGL::Viewport();

    kvs::OpenGL::WithPushedMatrix p( GL_MODELVIEW );
    p.loadIdentity();
    {
        m_hmd.beginFrame( frame_index );

        scene()->background()->apply();
        scene()->updateGLLightParameters();

        const kvs::Vec3 camera_position = scene()->camera()->position();
        const kvs::Vec3 camera_lookat = scene()->camera()->lookAt();
        const kvs::Vec3 camera_upvector = scene()->camera()->upVector();

        const OVR::Vector3f position0 = ::ToVec3( camera_position );
        const OVR::Vector3f lookat0 = ::ToVec3( camera_lookat );
        const OVR::Vector3f upvector0 = ::ToVec3( camera_upvector );
        const OVR::Matrix4f rotation0 = ::ToMat4( scene()->camera()->xform().rotation() );

        const size_t neyes = ovrEye_Count;
        for ( size_t i = 0; i < neyes; i++ )
        {
            const ovrEyeRenderDesc& render_desc = m_hmd.renderDesc( i );
            const ovrPosef& eye_pose = m_hmd.eyePose( i );
            const ovrRecti& viewport = m_hmd.viewport( i );

            // Set viewport.
            const ovrVector2i pos = viewport.Pos;
            const ovrSizei size = viewport.Size;
            kvs::OpenGL::SetViewport( pos.x, pos.y, size.w, size.h );

            // Set projection matrix.
            const float aspect = static_cast<float>( size.w ) / size.h;
            const float front = scene()->camera()->front();
            scene()->camera()->setWindowSize( size.w, size.h );
            if ( aspect >= 1.0f )
            {
                scene()->camera()->setTop( render_desc.Fov.UpTan * front );
                scene()->camera()->setBottom( -render_desc.Fov.DownTan * front );
                scene()->camera()->setLeft( -render_desc.Fov.LeftTan * front / aspect );
                scene()->camera()->setRight( render_desc.Fov.RightTan * front / aspect );
            }
            else
            {
                scene()->camera()->setTop( render_desc.Fov.UpTan * front * aspect );
                scene()->camera()->setBottom( -render_desc.Fov.DownTan * front * aspect );
                scene()->camera()->setLeft( -render_desc.Fov.LeftTan * front );
                scene()->camera()->setRight( render_desc.Fov.RightTan * front );
            }
            scene()->updateGLProjectionMatrix();

            // Set viewing matrix.
            const float scale = 10.0f;
            const OVR::Matrix4f R0 = rotation0;
            const OVR::Matrix4f R = R0 * OVR::Matrix4f( ts.HeadPose.ThePose.Orientation );
            const OVR::Vector3f T = R0.Transform( ts.HeadPose.ThePose.Position ) * scale;
            const OVR::Vector3f upvector = R.Transform( upvector0 );
            const OVR::Vector3f forward = R.Transform( lookat0 - position0 );
            const OVR::Vector3f position = position0 + T + R.Transform( eye_pose.Position ) * scale;
            const OVR::Vector3f lookat = position + forward;
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

        m_hmd.endFrame( frame_index );
    }

    // Render to the frame buffer.
    kvs::OpenGL::SetViewport( current_viewport );
    switch ( m_mirror_buffer_type )
    {
    case LeftEyeImage: m_hmd.renderToMirrorLeftEyeImage(); break;
    case RightEyeImage: m_hmd.renderToMirrorRightEyeImage(); break;
    case BothEyeImage: m_hmd.renderToMirrorBothEyeImage(); break;
    case DistortedBothEyeImage: m_hmd.renderToMirrorDistortedBothEyeImage(); break;
    default: break;
    }

    // Callback the registered paint events.
    kvs::PaintEvent event;
    BaseClass::eventHandler()->notify( &event );

    // Redraw.
    kvs::OpenGL::Flush();
    redraw();
}

} // end of namespace oculus

} // end of namespace kvs
