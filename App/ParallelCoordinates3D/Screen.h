#if defined( KVS_SUPPORT_OCULUS )
#include <Lib/Screen.h>
namespace local { typedef kvs::oculus::Screen Screen; }
#else
#include <kvs/glut/Screen>
namespace local { typedef kvs::glut::Screen Screen; }
#endif
