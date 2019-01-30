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
public:
    enum MirrorBufferType
    {
        LeftEyeImage = 0,
        RightEyeImage,
        BothEyeImage,
        DistortedBothEyeImage
    };

private:
    kvs::oculus::HeadMountedDisplay m_hmd; ///< Oculus HMD
    MirrorBufferType m_mirror_buffer_type; ///< mirror buffer type

public:
    Screen( kvs::oculus::Application* app = 0 );
    virtual ~Screen();

    const kvs::oculus::HeadMountedDisplay& headMountedDisplay() const { return m_hmd; }
    void setMirrorBufferType( const MirrorBufferType type ) { m_mirror_buffer_type = type; }
    void setMirrorBufferTypeToLeftEyeImage() { this->setMirrorBufferType( LeftEyeImage ); }
    void setMirrorBufferTypeToRightEyeImage() { this->setMirrorBufferType( RightEyeImage ); }
    void setMirrorBufferTypeToBothEyeImage() { this->setMirrorBufferType( BothEyeImage ); }
    void setMirrorBufferTypeToDistortedBothEyeImage() { this->setMirrorBufferType( DistortedBothEyeImage ); }

    virtual void show( const bool fullscreen = true );
    virtual void initializeEvent();
    virtual void paintEvent();
};

} // end of namespace oculus

} // end of namespace kvs
