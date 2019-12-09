#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/ObjectBase>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/TableObject>


namespace local
{

class ParallelCoordinates3DAxis : public kvs::RendererBase
{
    kvsModule( local::ParallelCoordinates3DAxis, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    mutable bool m_enable_anti_aliasing; ///< flag for anti-aliasing (AA)

public:
    ParallelCoordinates3DAxis();

    void setEnabledAntiAliasing( const bool enable ) const { m_enable_anti_aliasing = enable; }
    void enableAntiAliasing() const { this->setEnabledAntiAliasing( true ); }
    void disableAntiAliasing() const { this->setEnabledAntiAliasing( false ); }
    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:
    void draw_planes( const kvs::TableObject* table );
};

} // end of namespace local
