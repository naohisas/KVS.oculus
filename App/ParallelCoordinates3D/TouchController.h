#if defined( KVS_SUPPORT_OCULUS )
#include <Lib/TouchController.h>
namespace local { typedef kvs::oculus::TouchController TouchController; }
#else
#include <kvs/glut/Screen>
namespace local { struct TouchController { TouchController( const kvs::glut::Screen* ) {} }; }
#endif
