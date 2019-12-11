#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/ObjectBase>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/TableObject>


namespace local
{

class ParallelCoordinates3DRenderer : public kvs::RendererBase
{
    kvsModule( local::ParallelCoordinates3DRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    mutable bool m_enable_anti_aliasing; ///< flag for anti-aliasing (AA)

public:
    ParallelCoordinates3DRenderer();

    void setEnabledAntiAliasing( const bool enable ) const { m_enable_anti_aliasing = enable; }
    void enableAntiAliasing() const { this->setEnabledAntiAliasing( true ); }
    void disableAntiAliasing() const { this->setEnabledAntiAliasing( false ); }
    bool isEnabledAntiAliasing() const { return m_enable_anti_aliasing; }
    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

protected:
    void drawPoints( const kvs::TableObject* table );
    void drawLines( const kvs::TableObject* table );
};

} // end of namespace local
