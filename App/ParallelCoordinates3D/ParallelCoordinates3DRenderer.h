#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/ObjectBase>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/TableObject>
#include <kvs/RGBColor>


namespace local
{

class ParallelCoordinates3DRenderer : public kvs::RendererBase
{
    kvsModule( local::ParallelCoordinates3DRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    mutable bool m_enable_anti_aliasing; ///< flag for anti-aliasing (AA)
    float m_point_size;
    kvs::RGBColor m_point_color;
    float m_line_size;
    kvs::RGBColor m_line_color;

public:
    ParallelCoordinates3DRenderer();

    void setEnabledAntiAliasing( const bool enable ) const { m_enable_anti_aliasing = enable; }
    void enableAntiAliasing() const { this->setEnabledAntiAliasing( true ); }
    void disableAntiAliasing() const { this->setEnabledAntiAliasing( false ); }
    bool isEnabledAntiAliasing() const { return m_enable_anti_aliasing; }
    void setPointSize( const float size ) { m_point_size = size; }
    void setPointColor( const kvs::RGBColor& color ) { m_point_color = color; }
    void setLineSize( const float size ) { m_line_size = size; }
    void setLineColor( const kvs::RGBColor& color ) { m_line_color = color; }
    float pointSize() const { return m_point_size; }
    const kvs::RGBColor& pointColor() const { return m_point_color; }
    float lineSize() const { return m_line_size; }
    const kvs::RGBColor& lineColor() const { return m_line_color; }
    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

protected:
    void drawPoints( const kvs::TableObject* table, const float dpr = 1.0f );
    void drawLines( const kvs::TableObject* table, const float dpr = 1.0f );
};

} // end of namespace local
