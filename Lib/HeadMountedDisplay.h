#pragma once

#include "Oculus.h"


namespace kvs
{

namespace oculus
{

class HeadMountedDisplay
{
private:
    ovrHmd m_handler;

public:
    static int Detect();

public:
    HeadMountedDisplay();

    void create( int index = 0 );
    void destroy();
};

} // end of namespace oculus

} // end of namepsace kvs
