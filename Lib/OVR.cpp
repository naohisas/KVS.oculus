#include "OVR.h"
#include "Call.h"
#include "Version.h"
#include <kvs/Assert>
#include <kvs/Macro>


namespace kvs
{

namespace oculus
{

bool Initialize()
{
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 6, 0 )
    ovrResult result;
    KVS_OVR_CALL( result = ovr_Initialize( NULL ) );
    return OVR_SUCCESS( result );
#else
    ovrBool result;
    KVS_OVR_CALL( result = ovr_Initialize( NULL ) );
    return result == ovrTrue;
#endif
}

void Shutdown()
{
    KVS_OVR_CALL( ovr_Shutdown() );
}

int Detect()
{
#if KVS_OVR_VERSION_GREATER_OR_EQUAL( 0, 7, 0 )
    ovrHmdDesc desc;
    KVS_OVR_CALL( desc = ovr_GetHmdDesc( NULL ) );
    return desc.Type == ovrHmd_None ? 0 : 1;
#else
    int result = 0;
    KVS_OVR_CALL( result = ovrHmd_Detect() );
    return result;
#endif
}

std::string VersionString()
{
    std::string result;
    KVS_OVR_CALL( result = ovr_GetVersionString() );
    return result;
}

double TimeInSecond()
{
    double result;
    KVS_OVR_CALL( result = ovr_GetTimeInSeconds() );
    return result;
}

int TraceMessage( int level, const char* message )
{
    int result;
    KVS_OVR_CALL( result = ovr_TraceMessage( level, message ) );
    return result;
}

} // end of namespace oculus

} // end of namespace kvs
