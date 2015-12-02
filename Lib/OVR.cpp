#include "OVR.h"
#include <kvs/Assert>
#include <kvs/Macro>


namespace kvs
{

namespace oculus
{

namespace internal
{

bool HasError( ovrHmd hmd, const char* file, const int line, const char* func, const char* command )
{
    KVS_ASSERT( hmd );
    const char* error = ovrHmd_GetLastError( hmd );
    if ( error == NULL ) { return false; }

    // Output message tag.
    std::string message_tag( "KVS OVR ERROR" );
    std::cerr << KVS_MESSAGE_SET_COLOR( KVS_MESSAGE_RED );
    std::cerr << message_tag;
    std::cerr << KVS_MESSAGE_RESET_COLOR;

    // Output message with an error string.
    std::string error_string;
    while ( *error ) error_string += *error++;
    std::cerr << ": " << error_string << std::endl;
    std::cerr << "\t" << "FILE: " << file << " (" << line << ")" << std::endl;
    std::cerr << "\t" << "FUNC: " << func << std::endl;
    std::cerr << "\t" << "OVR COMMAND: " << command << std::endl;

    return true;
}

}

bool Initialize( ovrInitParams const* params )
{
    return ovr_Initialize( params ) == ovrTrue;
}

void Shutdown()
{
    ovr_Shutdown();
}

std::string VersionString()
{
    return std::string( ovr_GetVersionString() );
}

bool InitializeRenderingShimVersion( int requested_minor_version )
{
    return ovr_InitializeRenderingShimVersion( requested_minor_version ) == ovrTrue;
}

bool InitializeRenderingShim()
{
    return ovr_InitializeRenderingShim() == ovrTrue;
}

double TimeInSecond()
{
    return ovr_GetTimeInSeconds();
}

int TraceMessage( int level, const char* message )
{
    return ovr_TraceMessage( level, message );
}

} // end of namespace oculus

} // end of namespace kvs
