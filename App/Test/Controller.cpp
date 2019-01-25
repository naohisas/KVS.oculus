#include "Controller.h"
#include <Lib/OVR.h>
#include <Lib/Call.h>


namespace kvs
{

namespace oculus
{

Controller::Controller( const kvs::oculus::HeadMountedDisplay& hmd ):
    m_hmd( hmd ),
    m_types( 0 )
{
}

void Controller::initialize()
{
    KVS_OVR_CALL( m_types = ovr_GetConnectedControllerTypes( m_hmd.handler() ) );

    if ( !this->hasRight() ) { std::cout << "Cannot find Oculus touch controller (Right)." << std::endl; }
    if ( !this->hasLeft() ) { std::cout << "Cannot find Oculus touch controller (Left)." << std::endl; }
}

bool Controller::hasRight() const
{
    return ( m_types & ovrControllerType_RTouch ) == ovrControllerType_RTouch;
}

bool Controller::hasLeft() const
{
    return ( m_types & ovrControllerType_LTouch ) == ovrControllerType_LTouch;
}

ovrTrackingState Controller::trackingState( const kvs::Int64 frame_index ) const
{
    const double frame_timing = m_hmd.frameTiming( frame_index );
    return m_hmd.trackingState( frame_timing );
}

ovrInputState Controller::inputState( const ovrControllerType type ) const
{
    return m_hmd.inputState( type );
}

} // end of namespace oculus

} // end of namespace kvs