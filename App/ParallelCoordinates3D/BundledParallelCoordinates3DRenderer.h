#pragma once
#include "ParallelCoordinates3DRenderer.h"
#include <kvs/ColorMap>


namespace local
{

class BundledParallelCoordinates3DRenderer : public local::ParallelCoordinates3DRenderer
{
    kvsModule( local::BundledParallelCoordinates3DRenderer, Renderer );
    kvsModuleBaseClass( local::ParallelCoordinates3DRenderer );
public:
    float m_reduced_plane_scale;
    float m_curve_size;
private:
    
    
    kvs::ValueTable<float> m_reduced_data;
    kvs::ValueArray<float> m_reduced_min_values;
    kvs::ValueArray<float> m_reduced_max_values;
    size_t m_bundled_position;
    float m_bundled_line_size;
    kvs::RGBColor m_bundled_line_color;
    size_t m_nclusters;
    kvs::ValueArray<kvs::UInt32> m_clustered_color_ids;
    kvs::ColorMap m_clustered_colors;

public:
    BundledParallelCoordinates3DRenderer();

    void setReducedPlaneScale( const float scale ) { m_reduced_plane_scale = scale; }
    void setCurveSize( const float size ) { m_curve_size = size; }
    void setReducedData( const kvs::ValueTable<float>& data )
    {
        m_reduced_data = data;
        m_reduced_min_values.allocate( data.columnSize() );
        m_reduced_max_values.allocate( data.columnSize() );
        for ( size_t i = 0; i < data.columnSize(); i++ )
        {
            auto result = std::minmax_element( m_reduced_data[i].begin(), m_reduced_data[i].end() );
            m_reduced_min_values[i] = *result.first;
            m_reduced_max_values[i] = *result.second;
        }
    }
    void setBundledPosition( const size_t p ) { m_bundled_position = p; }
    void setBundledLineSize( const float size ) { m_bundled_line_size = size; }
    void setBundledLineColor( const kvs::RGBColor& color ) { m_bundled_line_color = color; }

    void setNumberOfClusters( const size_t nclusters ) { m_nclusters = nclusters; }
    void setClusterColorIDs( const kvs::ValueArray<kvs::UInt32>& ids ) { m_clustered_color_ids = ids; }
    void setClusterColors( const kvs::ColorMap& cmap ) { m_clustered_colors = cmap; }

    float reducedPlaneScale() const { return m_reduced_plane_scale; }
    const kvs::ValueTable<float>& reducedData() const { return m_reduced_data; }
    size_t bundledPosition() const { return m_bundled_position; }
    float bundledLineSize() const { return m_bundled_line_size; }
    const kvs::RGBColor& bundledLineColor() const { return m_bundled_line_color; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:
    void draw_bundled_lines( const kvs::TableObject* table, const float dpr = 1.0f );
};

} // end of namespace local
