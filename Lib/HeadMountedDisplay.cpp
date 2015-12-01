#include "HeadMountedDisplay.h"


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

void HeadMountedDisplay::destroy()
{
    ovrHmd_Destroy( m_handler );
}

} // end of namespace oculus

} // end of namespace kvs
