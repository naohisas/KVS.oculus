#pragma once
#include <Lib/HeadMountedDisplay.h>


namespace kvs
{

namespace oculus
{

class Controller
{
private:
    const kvs::oculus::HeadMountedDisplay& m_hmd; ///< hmd
    unsigned int m_types; ///< controller types connected to the hmd

public:
    Controller( const kvs::oculus::HeadMountedDisplay& hmd );

    void initialize();
    bool hasRight() const;
    bool hasLeft() const;
    ovrTrackingState trackingState( const kvs::Int64 frame_index = 0 ) const;
    ovrInputState inputState( const ovrControllerType type ) const;
};

} // end of namespace oculus

} // end of namespace kvs