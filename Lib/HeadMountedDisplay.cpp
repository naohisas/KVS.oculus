#include "HeadMountedDisplay.h"
#include "Oculus.h"
#include "OVR.h"
#include "Call.h"
#include <kvs/Platform>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

namespace oculus
{

HeadMountedDisplay::HeadMountedDisplay():
    m_handler( 0 ),
    m_descriptor( 0 )
{
}

kvs::UInt32 HeadMountedDisplay::availableHmdCaps() const
{
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 7, 0 )
    return m_descriptor->AvailableHmdCaps;

#else // 0.5.0 - 0.6.0
    return m_descriptor->HmdCaps;
#endif
}

kvs::UInt32 HeadMountedDisplay::availableTrackingCaps() const
{
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 7, 0 )
    return m_descriptor->AvailableTrackingCaps;

#else // 0.5.0 - 0.6.0
    return m_descriptor->TrackingCaps;
#endif
}

kvs::UInt32 HeadMountedDisplay::defaultHmdCaps() const
{
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 7, 0 )
    return m_descriptor->DefaultHmdCaps;

#else // 0.5.0 - 0.6.0
    return m_descriptor->HmdCaps;
#endif
}

kvs::UInt32 HeadMountedDisplay::defaultTrackingCaps() const
{
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 7, 0 )
    return m_descriptor->DefaultTrackingCaps;

#else // 0.5.0 - 0.6.0
    return m_descriptor->TrackingCaps;
#endif
}

bool HeadMountedDisplay::create( const int index )
{
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 7, 0 )
    ovrGraphicsLuid luid;
    ovrResult result;
    KVS_OVR_CALL( result = ovr_Create( &m_handler, &luid ) );
    if ( OVR_FAILURE( result ) ) { return false; }

    m_descriptor = new ovrHmdDesc;
    KVS_OVR_CALL( *m_descriptor = ovr_GetHmdDesc( m_handler ) );

#elif KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 6, 0 )
    ovrResult result;
    KVS_OVR_CALL( result = ovrHmd_Create( index, &m_handler ) );
    if ( OVR_FAILURE( result ) )
    {
        KVS_OVR_CALL( ovrHmd_CreateDebug( ovrHmd_DK1, &m_handler ) );
        if ( !m_handler ) { return false; }
    }

    m_descriptor = m_handler;

#else // 0.5.0
    KVS_OVR_CALL( m_handler = ovrHmd_Create( index ) );
    if ( !m_handler )
    {
        KVS_OVR_CALL( m_handler = ovrHmd_CreateDebug( ovrHmd_DK1 ) );
        if ( !m_handler ) { return false; }
    }

    m_descriptor = m_handler;

#endif
    return true;
}

void HeadMountedDisplay::destroy()
{
    if ( m_handler )
    {
        if ( m_mirror_fbo ) { KVS_GL_CALL( glDeleteFramebuffers( 1, &m_mirror_fbo ) ); }
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 6, 0 )
        if ( m_mirror_tex ) { KVS_OVR_CALL( ovr_DestroyMirrorTexture( m_handler, m_mirror_tex ) ); }
#else
        if ( m_mirror_tex ) { KVS_OVR_CALL( ovrHmd_DestroyMirrorTexture( m_handler, m_mirror_tex ) ); }
#endif

#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 1, 0, 0 )
        KVS_OVR_CALL( ovr_DestroyTextureSwapChain( m_handler, m_layer_data.ColorTexture[0] ) );
#elif KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 6, 0 )
        KVS_OVR_CALL( ovr_DestroySwapTextureSet( m_handler, m_layer_data.ColorTexture[0] ) );
#endif

#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 7, 0 )
        KVS_OVR_CALL( ovr_Destroy( m_handler ) );
        if ( m_descriptor ) { delete m_descriptor; }

#else // 0.5.0 - 0.6.0
        KVS_OVR_CALL( ovrHmd_Destroy( m_handler ) );
#endif
        m_handler = 0;
        m_descriptor = 0;
    }
}

void HeadMountedDisplay::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Product name: " << this->productName() << std::endl;
    os << indent << "Manufacture: " << this->manufacturer() << std::endl;
    os << indent << "Vender ID: " << this->vendorId() << std::endl;
    os << indent << "Product ID: " << this->productId() << std::endl;
    os << indent << "Resolution: " << kvs::oculus::ToVec2i( this->resolution() ) << std::endl;
}

bool HeadMountedDisplay::configureTracking( const kvs::UInt32 supported_caps, const kvs::UInt32 required_caps )
{
#if ( OVR_RPODUCT_VERSION >= 1 ) // Oculus SDK 1.x.x
    kvs::IgnoreUnusedVariable( supported_caps );
    kvs::IgnoreUnusedVariable( required_caps );
    ovrResult result;
    KVS_OVR_CALL( result = ovr_SetTrackingOriginType( m_handler, ovrTrackingOrigin_FloorLevel ) );
    return OVR_SUCCESS( result );

#else // Oculus SDK 0.x.x
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 8, 0 )
    // Usage of ovr_ConfigureTracking is no longer needed unless you want to disable tracking features.
    // By default, ovr_Create enables the full tracking capabilities supported by any given device.
    kvs::IgnoreUnusedVariable( supported_caps );
    kvs::IgnoreUnusedVariable( required_caps );
    return true;

#elif KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 7, 0 )
    ovrResult result;
    KVS_OVR_CALL( result = ovr_ConfigureTracking( m_handler, supported_caps, required_caps ) );
    return OVR_SUCCESS( result );

#else // 0.5.0 - 0.6.0
    ovrBool result;
    KVS_OVR_CALL( result = ovrHmd_ConfigureTracking( m_handler, supported_caps, required_caps ) );
    return result == ovrTrue;
#endif
#endif
}

bool HeadMountedDisplay::configureRendering()
{
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 7, 0 )
    KVS_OVR_CALL( m_render_descs[0] = ovr_GetRenderDesc( m_handler, ovrEye_Left, this->defaultEyeFov(0) ) );
    KVS_OVR_CALL( m_render_descs[1] = ovr_GetRenderDesc( m_handler, ovrEye_Right, this->defaultEyeFov(1) ) );

#elif KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 6, 0 )
    KVS_OVR_CALL( m_render_descs[0] = ovrHmd_GetRenderDesc( m_handler, ovrEye_Left, this->defaultEyeFov(0) ) );
    KVS_OVR_CALL( m_render_descs[1] = ovrHmd_GetRenderDesc( m_handler, ovrEye_Right, this->defaultEyeFov(1) ) );

#else // 0.5.0
    ovrGLConfig config;
    config.OGL.Header.API = ovrRenderAPI_OpenGL;
    config.OGL.Header.BackBufferSize = OVR::Sizei( this->resolution().w, this->resolution().h );
    config.OGL.Header.Multisample = 1;
#if defined( KVS_PLATFORM_WINDOWS )
    HDC dc = wglGetCurrentDC();
    HWND wnd = WindowFromDC( dc );
    if ( !( this->hmdCaps() & ovrHmdCap_ExtendDesktop ) )
    {
        KVS_OVR_CALL( ovrHmd_AttachToWindow( m_handler, wnd, NULL, NULL ) );
    }
    config.OGL.Window = wnd;
    config.OGL.DC = dc;
#endif
    ovrFovPort fov[2] = { this->defaultEyeFov(0), this->defaultEyeFov(1) };
    kvs::UInt32 caps = ovrDistortionCap_Vignette | ovrDistortionCap_TimeWarp | ovrDistortionCap_Overdrive;
    ovrBool result = ovrTrue;
    KVS_OVR_CALL( result = ovrHmd_ConfigureRendering( m_handler, &config.Config, caps, fov, m_render_descs ) );
    if ( result == ovrFalse ) { return false; }

    KVS_OVR_CALL( ovrHmd_SetEnabledCaps( m_handler, ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction ) );
    KVS_OVR_CALL( ovrHmd_DismissHSWDisplay( m_handler ) );
#endif

    this->update_viewport();
    return this->initialize_render_texture() && this->initialize_mirror_texture();
}

void HeadMountedDisplay::beginFrame( const kvs::Int64 frame_index )
{
#if ( KVS_OVR_MAJOR_VERSION >= 1 ) // Oculus SDK 1.x.x
    ovrResult result;
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 1, 19, 0 )
    KVS_OVR_CALL( result = ovr_WaitToBeginFrame( m_handler, frame_index ) );
    KVS_OVR_CALL( result = ovr_BeginFrame( m_handler, frame_index ) );
    KVS_ASSERT( OVR_SUCCESS( result ) );
#endif

    // Get the crrent color texture ID.
    const ovrTextureSwapChain& color_textures = m_layer_data.ColorTexture[0];
    int current_index = 0;
    KVS_OVR_CALL( result = ovr_GetTextureSwapChainCurrentIndex( m_handler, color_textures, &current_index ) );
    KVS_ASSERT( OVR_SUCCESS( result ) );

    // Bind the frame buffer object.
    m_framebuffer.bind();
    GLuint tex_id = 0;
    KVS_OVR_CALL( result = ovr_GetTextureSwapChainBufferGL( m_handler, color_textures, current_index, &tex_id ) );
    KVS_ASSERT( OVR_SUCCESS( result ) );
    KVS_GL_CALL( glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0 ) );

#else // Oculus SDK 0.x.x
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 6, 0 )
    // ovrHmd_BeginFrame was removed and ovrHmd_EndFrame was replaced with ovrHmd_SubmitFrame.
    // A list of layer pointers is passed into ovrHmd_SubmitFrame.
    kvs::IgnoreUnusedVariable( frame_index );

    // Get the crrent color texture ID.
    ovrSwapTextureSet* color_textures = m_layer_data.ColorTexture[0];
    const int texture_count = color_textures->TextureCount;
    const int prev_index = color_textures->CurrentIndex;
    const int current_index = ( prev_index + 1 ) % texture_count;
    color_textures->CurrentIndex = current_index;

    // Bind the frame buffer object.
    m_framebuffer.bind();
    const GLuint tex_id = reinterpret_cast<ovrGLTexture *>( &color_textures->Textures[ current_index ] )->OGL.TexId;
    KVS_GL_CALL( glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0 ) );

#else // 0.5.0
    ovrFrameTiming result;
    KVS_OVR_CALL( result = ovrHmd_BeginFrame( m_handler, frame_index ) );

    // Bind the frame buffer object.
    m_framebuffer.bind();
#endif
#endif

    this->update_eye_poses( frame_index );
}

void HeadMountedDisplay::endFrame( const kvs::Int64 frame_index )
{
#if KVS_OVR_VERSION_LESS_OR_EQUAL( 0, 5, 0 ) // Oculus SDK 0.5.0
    kvs::IgnoreUnusedVariable( frame_index );
    m_framebuffer.unbind();

    const ovrTexture* color_texture = &m_color_textures[0].Texture;
    KVS_OVR_CALL( ovrHmd_EndFrame( m_handler, m_eye_poses, color_texture ) );

#else // 0.6.0 - 1.x.x
    m_framebuffer.unbind();
    KVS_OVR_CALL( ovr_CommitTextureSwapChain( m_handler, m_layer_data.ColorTexture[0] ) );

    // Set view-scale descriptor.
    ovrViewScaleDesc view_scale_desc;
    view_scale_desc.HmdSpaceToWorldScaleInMeters = 1.0f;
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 1, 17, 0 )
    view_scale_desc.HmdToEyePose[0] = m_render_descs[0].HmdToEyePose;
    view_scale_desc.HmdToEyePose[1] = m_render_descs[1].HmdToEyePose;
#elif KVS_OVR_VERSION_GREATER_OR_EQUAL( 1, 3, 0 )
    view_scale_desc.HmdToEyeOffset[0] = m_render_descs[0].HmdToEyeOffset;
    view_scale_desc.HmdToEyeOffset[1] = m_render_descs[1].HmdToEyeOffset;
#else
    view_scale_desc.HmdToEyeViewOffset[0] = m_render_descs[0].HmdToEyeViewOffset;
    view_scale_desc.HmdToEyeViewOffset[1] = m_render_descs[1].HmdToEyeViewOffset;
#endif

    // Set layer eye fov.
    ovrLayerEyeFov layer_eye_fov;
    layer_eye_fov.Header.Type = ovrLayerType_EyeFov;
    layer_eye_fov.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;
    layer_eye_fov.ColorTexture[0] = m_layer_data.ColorTexture[0];
    layer_eye_fov.ColorTexture[1] = m_layer_data.ColorTexture[1];
    layer_eye_fov.Viewport[0] = m_viewports[0];
    layer_eye_fov.Viewport[1] = m_viewports[1];
    layer_eye_fov.Fov[0] = this->defaultEyeFov(0);
    layer_eye_fov.Fov[1] = this->defaultEyeFov(1);
    layer_eye_fov.RenderPose[0] = m_eye_poses[0];
    layer_eye_fov.RenderPose[1] = m_eye_poses[1];

    ovrLayerHeader* layers = &layer_eye_fov.Header;
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 1, 19, 0 )
    KVS_OVR_CALL( ovr_EndFrame( m_handler, frame_index, &view_scale_desc, &layers, 1 ) );
#elif KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 7, 0 )
    KVS_OVR_CALL( ovr_SubmitFrame( m_handler, frame_index, &view_scale_desc, &layers, 1 ) );
#else
    KVS_OVR_CALL( ovrHmd_SubmitFrame( m_handler, frame_index, &view_scale_desc, &layers, 1 ) );
#endif
#endif
}

double HeadMountedDisplay::frameTiming( const kvs::Int64 frame_index ) const
{
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 8, 0 )
    double result;
    KVS_OVR_CALL( result = ovr_GetPredictedDisplayTime( m_handler, frame_index ) );
    return result;

#elif KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 6, 0 )
    ovrFrameTiming timing;
    KVS_OVR_CALL( timing = ovr_GetFrameTiming( m_handler, frame_index ) );
    return timing.DisplayMidpointSeconds;

#else // 0.5.0
    kvs::IgnoreUnusedVariable( frame_index );
    return kvs::oculus::TimeInSecond();
#endif
}

void HeadMountedDisplay::renderToMirrorLeftEyeImage()
{
    const bool distorted = false;
    this->bind_mirror_buffer( distorted );

    const int mirror_x = m_layer_data.Viewport[0].Pos.x;
    const int mirror_y = m_layer_data.Viewport[0].Pos.y;
    const int mirror_width = m_layer_data.Viewport[0].Size.w;
    const int mirror_height = m_layer_data.Viewport[0].Size.h;
    const kvs::Vec4i mirror_viewport( mirror_x, mirror_y, mirror_width, mirror_height );
    const bool flip = false;
    this->blit_mirror_buffer( kvs::Vec4( mirror_viewport ), flip );

    this->unbind_mirror_buffer();
}

void HeadMountedDisplay::renderToMirrorRightEyeImage()
{
    const bool distorted = false;
    this->bind_mirror_buffer( distorted );

    const int mirror_x = m_layer_data.Viewport[1].Pos.x;
    const int mirror_y = m_layer_data.Viewport[1].Pos.y;
    const int mirror_width = m_layer_data.Viewport[1].Size.w;
    const int mirror_height = m_layer_data.Viewport[1].Size.h;
    const kvs::Vec4i mirror_viewport( mirror_x, mirror_y, mirror_width, mirror_height );

    const bool flip = false;
    this->blit_mirror_buffer( kvs::Vec4( mirror_viewport ), flip );

    this->unbind_mirror_buffer();
}

void HeadMountedDisplay::renderToMirrorBothEyeImage()
{
    const bool distorted = false;
    this->bind_mirror_buffer( distorted );

    const int mirror_x = m_layer_data.Viewport[0].Pos.x;
    const int mirror_y = m_layer_data.Viewport[0].Pos.y;
    const int mirror_width = m_layer_data.Viewport[0].Size.w + m_layer_data.Viewport[1].Size.w;
    const int mirror_height = m_layer_data.Viewport[0].Size.h;
    const kvs::Vec4i mirror_viewport( mirror_x, mirror_y, mirror_width, mirror_height );
    const bool flip = false;
    this->blit_mirror_buffer( kvs::Vec4( mirror_viewport ), flip );

    this->unbind_mirror_buffer();
}

void HeadMountedDisplay::renderToMirrorDistortedBothEyeImage()
{
    const bool distorted = true;
    this->bind_mirror_buffer( distorted );

    const bool flip = true;
    const kvs::Vec4i mirror_viewport( 0, 0, this->resolution().w, this->resolution().h );
    this->blit_mirror_buffer( kvs::Vec4( mirror_viewport ), flip );

    this->unbind_mirror_buffer();
}

void HeadMountedDisplay::resetTracking()
{
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 1, 3, 0 )
    KVS_OVR_CALL( ovr_RecenterTrackingOrigin( m_handler ) );

#elif KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 7, 0 )
    KVS_OVR_CALL( ovr_RecenterPose( m_handler ) );

#else // 0.5.0 - 0.6.0
    KVS_OVR_CALL( ovrHmd_RecenterPose( m_handler ) );
#endif
}

ovrTrackingState HeadMountedDisplay::trackingState( const double absolute_time, const bool latency ) const
{
    ovrTrackingState result;
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 8, 0 )
    const ovrBool latency_marker = ( latency ) ? ovrTrue : ovrFalse;
    KVS_OVR_CALL( result = ovr_GetTrackingState( m_handler, absolute_time, latency_marker ) );

#elif KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 7, 0 )
    kvs::IgnoreUnusedVariable( latency );
    KVS_OVR_CALL( result = ovr_GetTrackingState( m_handler, absolute_time ) );

#else // 0.5.0 - 0.6.0
    kvs::IgnoreUnusedVariable( latency );
    KVS_OVR_CALL( result = ovrHmd_GetTrackingState( m_handler, absolute_time ) );
#endif
    return result;
}

ovrInputState HeadMountedDisplay::inputState( const ovrControllerType type ) const
{
    ovrInputState result;
    KVS_OVR_CALL( ovr_GetInputState( m_handler, type, &result ) );
    return result;
}

void HeadMountedDisplay::update_viewport()
{
    // Left eye info.
    const ovrEyeType eye0 = ovrEye_Left;
    const ovrFovPort fov0 = this->defaultEyeFov(0);
    const ovrSizei tex0 = this->fov_texture_size( eye0, fov0, 1.0f );

    // Right eye info.
    const ovrEyeType eye1 = ovrEye_Right;
    const ovrFovPort fov1 = this->defaultEyeFov(1);
    const ovrSizei tex1 = this->fov_texture_size( eye1, fov1, 1.0f );

    // Rendering buffer size.
    m_buffer_size.w = tex0.w + tex1.w;
    m_buffer_size.h = kvs::Math::Max( tex0.h, tex1.h );

    // Viewport of each eye.
    m_viewports[0].Pos = OVR::Vector2i( 0, 0 );
    m_viewports[0].Size = OVR::Sizei( m_buffer_size.w / 2, m_buffer_size.h );
    m_viewports[1].Pos = OVR::Vector2i( ( m_buffer_size.w + 1 ) / 2, 0 );
    m_viewports[1].Size = m_viewports[0].Size;

#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 6, 0 )
    m_layer_data.Viewport[0] = m_viewports[0];
    m_layer_data.Viewport[1] = m_viewports[1];

#else // 0.5.0
    m_color_textures[0].OGL.Header.API = ovrRenderAPI_OpenGL;
    m_color_textures[0].OGL.Header.TextureSize = m_buffer_size;
    m_color_textures[0].OGL.Header.RenderViewport = m_viewports[0];
    m_color_textures[1] = m_color_textures[0];
    m_color_textures[1].OGL.Header.RenderViewport = m_viewports[1];
#endif
}

bool HeadMountedDisplay::initialize_render_texture()
{
#if ( KVS_OVR_MAJOR_VERSION >= 1 ) // Oculus SDK 1.x.x
    const ovrTextureSwapChainDesc chain = {
        ovrTexture_2D, // Type
        OVR_FORMAT_R8G8B8A8_UNORM_SRGB, // Format
        1, // ArraySize
        m_buffer_size.w, // Width
        m_buffer_size.h, // Height
        1, // MipLevels
        1, // SampleCount
        ovrFalse, // StaticImage
        0, // MiscFlags
        0 // BindFlags
    };

    // Create color textures
    ovrResult result;
    ovrTextureSwapChain color_textures;
    KVS_OVR_CALL( result = ovr_CreateTextureSwapChainGL( m_handler, &chain, &color_textures ) );
    if ( OVR_FAILURE( result ) )
    {
        kvsMessageError("Cannot create color textures.");
        return false;
    }

    int texture_count = 0;
    KVS_OVR_CALL( result = ovr_GetTextureSwapChainLength( m_handler, color_textures, &texture_count ) );
    if ( OVR_FAILURE( result ) || !texture_count )
    {
        kvsMessageError("Cannot get number of color textures.");
        return false;
    }

    for ( int i = 0; i < texture_count; i++ )
    {
        GLuint tex_id = 0;
        KVS_OVR_CALL( result = ovr_GetTextureSwapChainBufferGL( m_handler, color_textures, i, &tex_id ) );
        KVS_ASSERT( OVR_FAILURE( result ) );
        KVS_GL_CALL( glBindTexture( GL_TEXTURE_2D, tex_id ) );
        KVS_GL_CALL( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
        KVS_GL_CALL( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
        KVS_GL_CALL( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
        KVS_GL_CALL( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
        KVS_GL_CALL( glBindTexture( GL_TEXTURE_2D, 0 ) );
    }

    m_layer_data.ColorTexture[0] = color_textures;
    m_layer_data.ColorTexture[1] = color_textures;

    // Create a depth buffer.
    m_depth_buffer.setInternalFormat( GL_DEPTH_COMPONENT24 );
    m_depth_buffer.create( m_buffer_size.w, m_buffer_size.h );

    // Create a frame buffer object.
    m_framebuffer.create();
    m_framebuffer.bind();
    m_framebuffer.attachDepthRenderBuffer( m_depth_buffer );
    m_framebuffer.unbind();

#else // Oculus SDK 0.x.x
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 6, 0 )
    // Create color textures
    ovrResult result;
    ovrSwapTextureSet* color_textures;
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 7, 0 )
    KVS_OVR_CALL( result = ovr_CreateSwapTextureSetGL(
                      m_handler,
                      GL_SRGB8_ALPHA8,
                      m_buffer_size.w, m_buffer_size.h,
                      &color_textures ) );
#else
    KVS_OVR_CALL( result = ovrHmd_CreateSwapTextureSetGL(
                      m_handler,
                      GL_RGBA,
                      m_buffer_size.w, m_buffer_size.h,
                      &color_textures ) );
#endif
    if ( OVR_FAILURE( result ) )
    {
        kvsMessageError("Cannot create color textures.");
        return false;
    }

    const int texture_count = color_textures->TextureCount;
    for ( int i = 0; i < texture_count; i++ )
    {
        ovrGLTexture* tex = (ovrGLTexture*)&color_textures->Textures[i];
        KVS_GL_CALL( glBindTexture( GL_TEXTURE_2D, tex->OGL.TexId ) );
        KVS_GL_CALL( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
        KVS_GL_CALL( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
        KVS_GL_CALL( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
        KVS_GL_CALL( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
        KVS_GL_CALL( glBindTexture( GL_TEXTURE_2D, 0 ) );
    }

    m_layer_data.ColorTexture[0] = color_textures;
    m_layer_data.ColorTexture[1] = color_textures;

    // Create a depth buffer.
    m_depth_buffer.setInternalFormat( GL_DEPTH_COMPONENT24 );
    m_depth_buffer.create( m_buffer_size.w, m_buffer_size.h );

    // Create a frame buffer object.
    m_framebuffer.create();
    m_framebuffer.bind();
    m_framebuffer.attachDepthRenderBuffer( m_depth_buffer );
    m_framebuffer.unbind();

#else
    // Create a color buffer.
    m_color_buffer.setMagFilter( GL_LINEAR );
    m_color_buffer.setMinFilter( GL_LINEAR );
    m_color_buffer.setWrapS( GL_CLAMP_TO_EDGE );
    m_color_buffer.setWrapT( GL_CLAMP_TO_EDGE );
    m_color_buffer.setPixelFormat( GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE );
    m_color_buffer.create( m_buffer_size.w, m_buffer_size.h );

    m_color_textures[0].OGL.TexId = m_color_buffer.id();
    m_color_textures[1].OGL.TexId = m_color_buffer.id();

    // Create a depth buffer.
    m_depth_buffer.setInternalFormat( GL_DEPTH_COMPONENT24 );
    m_depth_buffer.create( m_buffer_size.w, m_buffer_size.h );

    // Create a frame buffer object.
    m_framebuffer.create();
    m_framebuffer.bind();
    m_framebuffer.attachColorTexture( m_color_buffer );
    m_framebuffer.attachDepthRenderBuffer( m_depth_buffer );
    m_framebuffer.unbind();

#endif
#endif

    return true;
}

bool HeadMountedDisplay::initialize_mirror_texture()
{
    ovrResult result;
    const int width = this->resolution().w;
    const int height = this->resolution().h;

#if ( KVS_OVR_MAJOR_VERSION >= 1 ) // Oculus SDK 1.x.x
    // Mirror texture descriptor.
    const ovrMirrorTextureDesc mirror = {
        OVR_FORMAT_R8G8B8A8_UNORM_SRGB, // Format
        width, // Width
        height, // Height
        0
    };

    // Create a mirror texture.
    KVS_OVR_CALL( result = ovr_CreateMirrorTextureGL( m_handler, &mirror, &m_mirror_tex ) );
    if ( OVR_FAILURE( result ) )
    {
        kvsMessageError( "Cannot create mirror texture." );
        return false;
    }

    // Create a mirror frame buffer object.
    KVS_GL_CALL( glGenFramebuffers( 1, &m_mirror_fbo ) );

#else // Oculus SDK 0.x.x

    // Create a mirror texture.
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 6, 0 )
    KVS_OVR_CALL( result = ovr_CreateMirrorTextureGL( m_handler, GL_SRGB8_ALPHA8, width, height, reinterpret_cast<ovrTexture **>(&m_mirror_tex) ) );
#else
    KVS_OVR_CALL( result = ovrHmd_CreateMirrorTextureGL( m_handler, GL_SRGB8_ALPHA8, width, height, reinterpret_cast<ovrTexture **>(&m_mirror_tex) ) );
#endif
    if ( OVR_FAILURE( result ) )
    {
        kvsMessageError( "Cannot create mirror texture." );
        return false;
    }

    // Create a mirror frame buffer object.
    KVS_GL_CALL( glGenFramebuffers( 1, &m_mirror_fbo ) );
    KVS_GL_CALL( glBindFramebuffer( GL_READ_FRAMEBUFFER, m_mirror_fbo ) );
    KVS_GL_CALL( glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_mirror_txt->OGL.TexId, 0 ) );
    KVS_GL_CALL( glFramebufferRenderbuffer( GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0 ) );
    KVS_GL_CALL( glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 ) );
#endif

    return true;
}

void HeadMountedDisplay::update_eye_poses( const kvs::Int64 frame_index )
{
#if ( KVS_OVR_MAJOR_VERSION >= 1 ) // Oculus SDK 1.x.x
    const double timing = this->frameTiming( frame_index );
    const ovrTrackingState state = this->trackingState( timing );

#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 1, 17, 0 )
    const ovrPosef poses[2] = {
        m_render_descs[0].HmdToEyePose,
        m_render_descs[1].HmdToEyePose
    };
    KVS_OVR_CALL( ovr_CalcEyePoses( state.HeadPose.ThePose, poses, m_eye_poses ) );

#elif KVS_OVR_VERSION_GREATER_OR_EQUAL( 1, 3, 0 )
    const ovrVector3f offsets[2] = {
        m_render_descs[0].HmdToEyeOffset,
        m_render_descs[1].HmdToEyeOffset
    };
    KVS_OVR_CALL( ovr_CalcEyePoses( state.HeadPose.ThePose, offsets, m_eye_poses ) );

#else
    const ovrVector3f offsets[2] = {
        m_render_descs[0].HmdToEyeViewOffset,
        m_render_descs[1].HmdToEyeViewOffset
    };
    KVS_OVR_CALL( ovr_CalcEyePoses( state.HeadPose.ThePose, offsets, m_eye_poses ) );
#endif

#else // Oculus SDK 0.x.x
    const ovrVector3f offsets[2] = {
        m_render_descs[0].HmdToEyeViewOffset,
        m_render_descs[1].HmdToEyeViewOffset
    };

#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 6, 0 )
    const double timing = this->frameTiming( frame_index );
    const ovrTrackingState state = this->trackingState( timing );
    KVS_OVR_CALL( ovr_CalcEyePoses( state.HeadPose.ThePose, offsets, m_eye_poses ) );

#else // 0.5.0
    KVS_OVR_CALL( ovrHmd_GetEyePoses( m_handler, frame_index, offsets, m_eye_poses, NULL ) );
#endif

#endif
}

ovrSizei HeadMountedDisplay::fov_texture_size( const ovrEyeType eye, const ovrFovPort fov, const float pixels_per_display_pixel )
{
    ovrSizei result;
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 7, 0 )
    KVS_OVR_CALL( result = ovr_GetFovTextureSize( m_handler, eye, fov, pixels_per_display_pixel ) );

#else // 0.5.0 - 0.6.0
    KVS_OVR_CALL( result = ovrHmd_GetFovTextureSize( m_handler, eye, fov, pixels_per_display_pixel ) );
#endif
    return result;
}

void HeadMountedDisplay::bind_mirror_buffer( const bool distorted )
{
    kvs::OpenGL::SetDrawBuffer( GL_FRONT );
    kvs::OpenGL::SetClearColor( kvs::RGBColor::Black() );
    kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT );

    if ( distorted )
    {
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 1, 0, 0 )
        // Get the crrent color texture ID.
        GLuint tex_id = 0;
        KVS_OVR_CALL( ovr_GetMirrorTextureBufferGL( m_handler, m_mirror_tex, &tex_id ) );

        // Bind the frame buffer object.
        KVS_GL_CALL( glBindFramebuffer( GL_READ_FRAMEBUFFER, m_mirror_fbo ) );
        KVS_GL_CALL( glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0 ) );
#else
//        KVS_GL_CALL( glBindFramebuffer( GL_READ_FRAMEBUFFER, m_mirror_fbo ) );
//        KVS_GL_CALL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 ) );
#endif
    }
    else
    {
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 1, 0, 0 )
        // Get the crrent color texture ID.
        int current_index = 0;
        const ovrTextureSwapChain& color_textures = m_layer_data.ColorTexture[0];
        KVS_OVR_CALL( ovr_GetTextureSwapChainCurrentIndex( m_handler, color_textures, &current_index ) );

        GLuint tex_id = 0;
        KVS_OVR_CALL( ovr_GetTextureSwapChainBufferGL( m_handler, color_textures, current_index, &tex_id ) );

        // Bind the frame buffer object.
        KVS_GL_CALL( glBindFramebuffer( GL_READ_FRAMEBUFFER, m_framebuffer.id() ) );
        KVS_GL_CALL( glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0 ) );
#else
//        KVS_GL_CALL( glBindFramebuffer( GL_READ_FRAMEBUFFER, m_mirror_fbo ) );
//        KVS_GL_CALL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 ) );
#endif
    }
}

void HeadMountedDisplay::unbind_mirror_buffer()
{
    KVS_GL_CALL( glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 ) );
}

void HeadMountedDisplay::blit_mirror_buffer( const kvs::Vec4 mirror_viewport, const bool flip )
{
    const kvs::Vec4 screen_viewport = kvs::OpenGL::Viewport();
    const int screen_width = static_cast<int>( screen_viewport[2] );
    const int screen_height = static_cast<int>( screen_viewport[3] );

    const int mirror_x = static_cast<int>( mirror_viewport[0] );
    const int mirror_y = static_cast<int>( mirror_viewport[1] );
    const int mirror_width = static_cast<int>( mirror_viewport[2] );
    const int mirror_height = static_cast<int>( mirror_viewport[3] );

    const float mirror_ratio = static_cast<float>( mirror_width ) / mirror_height;
    const float screen_ratio = static_cast<float>( screen_width ) / screen_height;

    const int src_x0 = mirror_x;
    const int src_y0 = flip ? mirror_height : mirror_y;
    const int src_x1 = src_x0 + mirror_width;
    const int src_y1 = flip ? mirror_y: src_y0 + mirror_height;

    if ( screen_ratio > mirror_ratio )
    {
        const int width = static_cast<int>( screen_height * mirror_ratio );
        const int height = screen_height;
        const int dst_x0 = static_cast<int>( ( screen_width - width ) * 0.5f );
        const int dst_y0 = 0;
        const int dst_x1 = dst_x0 + width;
        const int dst_y1 = screen_height;
        KVS_GL_CALL( glBlitFramebuffer( src_x0, src_y0, src_x1, src_y1, dst_x0, dst_y0, dst_x1, dst_y1, GL_COLOR_BUFFER_BIT, GL_LINEAR ) );
    }
    else
    {
        const int width = screen_width;
        const int height = static_cast<int>( screen_width / mirror_ratio );
        const int dst_x0 = 0;
        const int dst_y0 = static_cast<int>( ( screen_height - height ) * 0.5f );
        const int dst_x1 = screen_width;
        const int dst_y1 = dst_y0 + screen_height;
        KVS_GL_CALL( glBlitFramebuffer( src_x0, src_y0, src_x1, src_y1, dst_x0, dst_y0, dst_x1, dst_y1, GL_COLOR_BUFFER_BIT, GL_LINEAR ) );
    }
}


} // end of namespace oculus

} // end of namespace kvs
