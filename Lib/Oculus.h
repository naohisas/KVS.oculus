#pragma once
#include <kvs/OpenGL>
#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>
#include <OVR_Version.h>
#include <Extras/OVR_Math.h>
#include <string>


namespace kvs
{

namespace oculus
{

inline const std::string Description()
{
    const std::string description("Oculus Rift SDK");
    return description;
}

inline const std::string Version()
{
    const std::string version( OVR_VERSION_STRING );
    return version;
}

} // end of namespace oculus

} // end of namespace kvs
