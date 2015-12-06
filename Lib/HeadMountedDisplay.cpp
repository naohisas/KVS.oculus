#include "HeadMountedDisplay.h"
#include "OVR.h"
#include <kvs/Macro>
#include <kvs/Breakpoint>


/*===========================================================================*/
/**
 *  @def KVS_OVR_CALL( command )
 *  A macro for OVR command safe calling. An error checking will be done
 *  before and after calling the command when the debug option (KVS_ENABLE_DEBUG)
 *  is available.
 */
/*===========================================================================*/
#if defined( KVS_ENABLE_DEBUG )
#define KVS_OVR_CALL( command )                                          \
    KVS_MACRO_MULTI_STATEMENT_BEGIN                                     \
    if ( kvs::oculus::internal::HasError( m_handler, KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC, "Unknown" ) ) { KVS_BREAKPOINT; } \
    command;                                                            \
    if ( kvs::oculus::internal::HasError( m_handler, KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC, #command ) ) { KVS_BREAKPOINT; } \
    KVS_MACRO_MULTI_STATEMENT_END
#else
#define KVS_OVR_CALL( command ) ( command )
#endif


namespace kvs
{

namespace oculus
{

int HeadMountedDisplay::Detect()
{
    return ovrHmd_Detect();
}

HeadMountedDisplay::HeadMountedDisplay():
    m_handler( 0 )
{
}

void HeadMountedDisplay::create( int index )
{
    m_handler = ovrHmd_Create( index );
}

void HeadMountedDisplay::createDebug( ovrHmdType type )
{
    m_handler = ovrHmd_CreateDebug( type );
}

void HeadMountedDisplay::destroy()
{
    if ( m_handler )
    {
        ovrHmd_Destroy( m_handler );
        m_handler = 0;
    }
}

std::string HeadMountedDisplay::lastError()
{
    return std::string( ovrHmd_GetLastError( m_handler ) );
}

bool HeadMountedDisplay::attachToWindow(
    void* window,
    const ovrRecti* dst_mirror_rect,
    const ovrRecti* src_render_target_rect )
{
    ovrBool ret;
    KVS_OVR_CALL( ret = ovrHmd_AttachToWindow( m_handler, window, dst_mirror_rect, src_render_target_rect ) );
    return ret == ovrTrue;
}

kvs::UInt32 HeadMountedDisplay::enabledCaps()
{
    kvs::UInt32 caps;
    KVS_OVR_CALL( caps = ovrHmd_GetEnabledCaps( m_handler ) );
    return caps;
}

void HeadMountedDisplay::setEnabledCaps( kvs::UInt32 caps )
{
    KVS_OVR_CALL( ovrHmd_SetEnabledCaps( m_handler, caps ) );
}

bool HeadMountedDisplay::configureTracking( kvs::UInt32 supported_caps, kvs::UInt32 required_caps )
{
    ovrBool ret;
    KVS_OVR_CALL( ret = ovrHmd_ConfigureTracking( m_handler, supported_caps, required_caps ) );
    return ret == ovrTrue;
}

void HeadMountedDisplay::recenterPose()
{
    KVS_OVR_CALL( ovrHmd_RecenterPose( m_handler ) );
}

ovrTrackingState HeadMountedDisplay::trackingState( double absolute_time )
{
    ovrTrackingState state;
    KVS_OVR_CALL( state = ovrHmd_GetTrackingState( m_handler, absolute_time ) );
    return state;
}

ovrSizei HeadMountedDisplay::fovTextureSize( ovrEyeType eye, ovrFovPort fov, float pixels_per_display_pixel )
{
    ovrSizei ret;
    KVS_OVR_CALL( ret = ovrHmd_GetFovTextureSize( m_handler, eye, fov, pixels_per_display_pixel ) );
    return ret;
}

bool HeadMountedDisplay::configureRendering(
    const ovrRenderAPIConfig* conf,
    kvs::UInt32 caps,
    const ovrFovPort fov[2],
    ovrEyeRenderDesc desc[2] )
{
    ovrBool ret;
    KVS_OVR_CALL( ret = ovrHmd_ConfigureRendering( m_handler, conf, caps, fov, desc ) );
    return ret == ovrTrue;
}

ovrFrameTiming HeadMountedDisplay::beginFrame( kvs::UInt32 index )
{
    ovrFrameTiming ret;
    KVS_OVR_CALL( ret = ovrHmd_BeginFrame( m_handler, index ) );
    return ret;
}

void HeadMountedDisplay::endFrame( const ovrPosef pose[2], const ovrTexture texture[2] )
{
    KVS_OVR_CALL( ovrHmd_EndFrame( m_handler, pose, texture ) );
}

void HeadMountedDisplay::getEyePoses( kvs::UInt32 index, const ovrVector3f offset[2], ovrPosef poses[2], ovrTrackingState* state )
{
    KVS_OVR_CALL( ovrHmd_GetEyePoses( m_handler, index, offset, poses, state ) );
}

ovrPosef HeadMountedDisplay::posePerEye( ovrEyeType eye )
{
    ovrPosef ret;
    KVS_OVR_CALL( ret = ovrHmd_GetHmdPosePerEye( m_handler, eye ) );
    return ret;
}

} // end of namespace oculus

} // end of namespace kvs
