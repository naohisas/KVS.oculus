#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/ObjectBase>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/TableObject>
#include <vector>


namespace local
{

class ParallelCoordinates3DAxis : public kvs::RendererBase
{
    kvsModule( local::ParallelCoordinates3DAxis, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    mutable bool m_enable_anti_aliasing; ///< flag for anti-aliasing (AA)
    std::vector<kvs::Vec3> m_plane_positions; ///< plane positions
    float m_axis_width;
    kvs::RGBColor m_axis_color;
    kvs::RGBColor m_label_color;
    kvs::RGBColor m_value_color;
    kvs::RGBAColor m_background_color;
    size_t m_number_of_grid;
    std::string m_x_label; ///< label of the x axis
    std::string m_y_label; ///< label of the y axis
    std::string m_z_label; ///< label of the z axis
    bool m_show_labels; ///< flags for showing the labels
    bool m_show_values; ///< flags for showing the values

public:
    ParallelCoordinates3DAxis();

    void setEnabledAntiAliasing( const bool enable ) const { m_enable_anti_aliasing = enable; }
    void enableAntiAliasing() const { this->setEnabledAntiAliasing( true ); }
    void disableAntiAliasing() const { this->setEnabledAntiAliasing( false ); }
    void setAxisWidth( const float width ) { m_axis_width = width; }
    void setAxisColor( const kvs::RGBColor& color ) { m_axis_color = color; }
    void setBackgroundColor( const kvs::RGBAColor& color ) { m_background_color = color; }
    float axisWidth() const { return m_axis_width; }
    const kvs::RGBColor& axisColor() const { return m_axis_color; }
    const kvs::RGBAColor& backgroundColor() const { return m_background_color; }
    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:
    void draw_planes( const kvs::TableObject* table, const float dpr = 1.0f );
    void draw_gridlines( const kvs::TableObject* table, const float dpr = 1.0f );
    void draw_labels( const kvs::TableObject* table, const float dpr = 1.0f );
};

} // end of namespace local
