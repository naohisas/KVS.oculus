#include "TouchEventListener.h"
#include <Lib/OVR.h>


namespace kvs
{

namespace oculus
{

TouchEventListener::TouchEventListener( kvs::oculus::Screen* screen ):
    kvs::oculus::EventListener( screen )
{
    if ( !controller().hasRight() ) { std::cout << "Oculus Touch (R): Disable" << std::endl; }
    if ( !controller().hasLeft() ) { std::cout << "Oculus Touch (L): Disable" << std::endl; }
}

void TouchEventListener::initializeEvent()
{
    std::cout << "TouchEventListener::initializeEvent()" << std::endl;
    if ( !controller().hasRight() ) { std::cout << "Oculus Touch (R): Disable" << std::endl; }
    if ( !controller().hasLeft() ) { std::cout << "Oculus Touch (L): Disable" << std::endl; }
}

void TouchEventListener::frameEvent()
{
    std::cout << "TouchEventListener::frameEvent()" << std::endl;
    if ( !controller().hasRight() ) { std::cout << "Oculus Touch (R): Disable" << std::endl; }
    if ( !controller().hasLeft() ) { std::cout << "Oculus Touch (L): Disable" << std::endl; }

    const ovrTrackingState ts = controller().trackingState( 0 );
    ovrPosef hands[2] = { ts.HandPoses[ovrHand_Right].ThePose, ts.HandPoses[ovrHand_Left].ThePose };

    std::cout << "  Position (R): " << kvs::oculus::ToVec3( hands[0].Position ) << std::endl;
    std::cout << "  Position (L): " << kvs::oculus::ToVec3( hands[1].Position ) << std::endl;
}

} // end of namespace oculus

} // end of namespace kvs