#if defined( KVS_SUPPORT_OCULUS )
#include <Lib/Application.h>
namespace local { typedef kvs::oculus::Application Application; }
#else
#include <kvs/glut/Application>
namespace local { typedef kvs::glut::Application Application; }
#endif
