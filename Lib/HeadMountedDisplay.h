#pragma once

#include "Oculus.h"
#include <string>
#include <kvs/Type>
#include <kvs/Vector2>


namespace kvs
{

namespace oculus
{

class HeadMountedDisplay
{
private:
    ovrHmd m_handler;

public:
    static int Detect();

public:
    HeadMountedDisplay();

    // Propaties
    const ovrHmd& handler() const { return m_handler; }
    std::string productName() const { return std::string( m_handler->ProductName ); }
    std::string manufacturer() const { return std::string( m_handler->Manufacturer ); }
    kvs::Int16 vendorId() const { return m_handler->VendorId; }
    kvs::Int16 productId() const { return m_handler->ProductId; }
    kvs::UInt32 hmdCaps() const { return m_handler->HmdCaps; }
    kvs::UInt32 trackingCaps() const { return m_handler->TrackingCaps; }
    kvs::UInt32 distortionCaps() const { return m_handler->DistortionCaps; }
    ovrFovPort defaultEyeFov( const size_t index ) const { return m_handler->DefaultEyeFov[index]; }
    ovrFovPort maxEyeFov( const size_t index ) const { return m_handler->MaxEyeFov[index]; }
    ovrEyeType eyeRenderOrder( const size_t index ) const { return m_handler->EyeRenderOrder[index]; }
    ovrSizei resolution() const { return m_handler->Resolution; }
    ovrVector2i windowPosition() const { return m_handler->WindowsPos; }

    bool create( int index = 0 );
    bool createDebug( ovrHmdType type = ovrHmd_DK1 );
    void destroy();
    std::string lastError();

    bool attachToWindow( void* window, const ovrRecti* dst_mirror_rect, const ovrRecti* src_render_target_rect );
    kvs::UInt32 enabledCaps();
    void setEnabledCaps( kvs::UInt32 caps );

    // Tracking interface methods
    bool configureTracking( kvs::UInt32 supported_caps, kvs::UInt32 required_caps );
    void recenterPose();
    ovrTrackingState trackingState( double absolute_time );

    // Graphics setup method
    ovrSizei fovTextureSize( ovrEyeType eye, ovrFovPort fov, float pixels_per_display_pixel );

    // SDK distortion rendering methods
    bool configureRendering( const ovrRenderAPIConfig* conf, kvs::UInt32 caps, const ovrFovPort fov[2], ovrEyeRenderDesc desc[2] );
    ovrFrameTiming beginFrame( kvs::UInt32 index = 0 );
    void endFrame( const ovrPosef pose[2], const ovrTexture texture[2] );
    void getEyePoses( kvs::UInt32 index, const ovrVector3f offset[2], ovrPosef poses[2], ovrTrackingState* state );
    ovrPosef posePerEye( ovrEyeType eye );
};

} // end of namespace oculus

} // end of namepsace kvs
