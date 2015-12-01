#pragma once
#include <kvs/ApplicationBase>


namespace kvs
{

namespace oculus
{

class Application : public kvs::ApplicationBase
{
public:
    Application( int argc, char** argv );

    virtual int run();
    virtual void quit();
};

} // end of namepace oculus

} // end of namespace kvs
