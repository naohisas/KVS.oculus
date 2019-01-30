#pragma once
#include "Oculus.h"
#include "Version.h"
#include <string>
#include <iostream>
#include <kvs/Type>
#include <kvs/Vector2>
#include <kvs/OpenGL>
#include <kvs/FrameBufferObject>
#include <kvs/RenderBuffer>
#include <kvs/Texture2D>
#include <kvs/Indent>


namespace kvs
{

namespace oculus
{

class HeadMountedDisplay
{
public:
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 8, 0 )
    typedef ovrSession Handler;
#else
    typedef ovrHmd Handler;
#endif

#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 7, 0 )
    typedef ovrHmdDesc* Descriptor;
#else
    typedef ovrHmd Descriptor;
#endif

private:
    Handler m_handler; ///< HMD device session handler
    Descriptor m_descriptor; ///< HMD device descriptor

    ovrEyeRenderDesc m_render_descs[2]; ///< rendering descriptors (rendering information of each eye)
    ovrPosef m_eye_poses[2]; ///< position and orientation of each eye
    ovrRecti m_viewports[2]; ///< viewport of each eye

    // Rendering buffers.
    ovrSizei m_buffer_size; ///< rendering buffer size
    kvs::FrameBufferObject m_framebuffer; ///< frame buffer object
    kvs::RenderBuffer m_depth_buffer; ///< depth buffer
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 6, 0 )
    ovrLayerEyeFov m_layer_data; ///< frame layer data
#else
    kvs::Texture2D m_color_buffer; ///< color buffer
    ovrGLTexture m_color_textures[2]; ///< texture information of each eye
#endif

    // Mirror buffer.
    GLuint m_mirror_fbo;
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 1, 0, 0 )
    ovrMirrorTexture m_mirror_tex;
#else
    ovrGLTexture* m_mirror_tex;
#endif

public:
    HeadMountedDisplay();

    const Handler& handler() const { return m_handler; }
    const ovrEyeRenderDesc& renderDesc( const size_t eye_index ) const { return m_render_descs[eye_index]; }
    const ovrPosef& eyePose( const size_t eye_index ) const { return m_eye_poses[eye_index]; }
    const ovrRecti& viewport( const size_t eye_index ) const { return m_viewports[eye_index]; }

    // Descriptor.
    std::string productName() const { return std::string( m_descriptor->ProductName ); }
    std::string manufacturer() const { return std::string( m_descriptor->Manufacturer ); }
    kvs::Int16 vendorId() const { return m_descriptor->VendorId; }
    kvs::Int16 productId() const { return m_descriptor->ProductId; }
    ovrFovPort defaultEyeFov( const size_t eye_index ) const { return m_descriptor->DefaultEyeFov[ eye_index ]; }
    ovrFovPort maxEyeFov( const size_t eye_index ) const { return m_descriptor->MaxEyeFov[ eye_index ]; }
    ovrSizei resolution() const { return m_descriptor->Resolution; }
    kvs::UInt32 availableHmdCaps() const;
    kvs::UInt32 availableTrackingCaps() const;
    kvs::UInt32 defaultHmdCaps() const;
    kvs::UInt32 defaultTrackingCaps() const;

    bool create( const int index = 0 );
    void destroy();
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;

    // Rendering
    bool configureRendering();
    void beginFrame( const kvs::Int64 frame_index );
    void endFrame( const kvs::Int64 frame_index );
    double frameTiming( const kvs::Int64 frame_index ) const;

    // Rendering to mirror buffer
    void renderToMirrorLeftEyeImage();
    void renderToMirrorRightEyeImage();
    void renderToMirrorBothEyeImage();
    void renderToMirrorDistortedBothEyeImage();

    // Tracking
    bool configureTracking( const kvs::UInt32 supported_caps, const kvs::UInt32 required_caps );
    void resetTracking();
    ovrTrackingState trackingState( const double absolute_time = 0.0, const bool latency = true ) const;
    ovrInputState inputState( const ovrControllerType type ) const;

private:
    bool initialize_render_texture();
    bool initialize_mirror_texture();
    void update_eye_poses( const kvs::Int64 frame_index );
    void update_viewport();
    ovrSizei fov_texture_size( const ovrEyeType eye, const ovrFovPort fov, const float pixels_per_display_pixel );
    void bind_mirror_buffer( const bool distorted = true );
    void unbind_mirror_buffer();
    void blit_mirror_buffer( const kvs::Vec4 mirror_viewport, const bool flip = false );
};

} // end of namespace oculus

} // end of namepsace kvs
