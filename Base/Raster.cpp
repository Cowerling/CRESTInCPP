//
// Created by cowerling on 17-4-19.
//
#include <cstring>
#include <cmath>

#include "Raster.h"
#include "ActualRaster.h"

using namespace CREST;

Raster::Raster()
{}

Raster::Raster(GDALDataset *dataset, GDALDataType data_type) : m_dataset(dataset), m_data_type(data_type)
{
    m_origin_x = m_origin_y = 0;
    m_x_extent = m_y_extent = 200;
    m_x_search_size = m_y_search_size = 0;

    m_x_size = dataset->GetRasterXSize();
    m_y_size = dataset->GetRasterYSize();

    dataset->GetGeoTransform(m_geo_transform);

    m_no_data_value = dataset->GetRasterBand(1)->GetNoDataValue();
}

Raster::~Raster()
{
    GDALClose(m_dataset);
}

Raster* Raster::CreateRaster(GDALDataset *dataset, int x_extent, int y_extent)
{
    GDALDataType data_type = dataset->GetRasterBand(1)->GetRasterDataType();
    Raster *raster = nullptr;

    switch (data_type)
    {
        case GDT_Byte:
            raster = new ActualRaster<char>(dataset, data_type);
            break;
        case GDT_UInt16:
            raster = new ActualRaster<unsigned short>(dataset, data_type);
            break;
        case GDT_Int16:
            raster = new ActualRaster<short>(dataset, data_type);
            break;
        case GDT_UInt32:
            raster = new ActualRaster<unsigned int>(dataset, data_type);
            break;
        case GDT_Int32:
            raster = new ActualRaster<int>(dataset, data_type);
            break;
        case GDT_Float32:
            raster = new ActualRaster<float>(dataset, data_type);
            break;
        case GDT_Float64:
        default:
            raster = new ActualRaster<double>(dataset, data_type);
            break;
    }

    raster->SetExtent(x_extent, y_extent);
    return raster;
}

void Raster::SetExtent(int x_extent, int y_extent)
{
    m_x_extent = x_extent;
    m_y_extent = y_extent;
}

bool Raster::Contain(int x, int y) const
{
    if (m_origin_x <= x && x < m_origin_x + m_x_search_size && m_origin_y <= y && y < m_origin_y + m_y_search_size)
        return true;
    else
        return false;
}

OGRSpatialReference Raster::GetSpatialReference() const
{
    const char *projectionRef = m_dataset->GetProjectionRef();

    int length = std::strlen(projectionRef);
    char *wkt = new char[length + 1];
    std::strcpy(wkt, projectionRef);
    wkt[length] = '\0';

    OGRSpatialReference spatial_reference;
    spatial_reference.importFromWkt(&wkt);
    //
    //delete wkt;
    //

    return spatial_reference;
}

void Raster::GetCoordinates(int x, int y, double &coordinate_x, double &coordinate_y)
{
    SpatialDriver::GetCoordinates(m_geo_transform, GetYSize(), x, y, coordinate_x, coordinate_y);
}

bool Raster::IsSameScope(const Raster &raster) const
{
    OGRSpatialReference spatial_reference = raster.GetSpatialReference();
    bool is_same_spatial_reference = this->GetSpatialReference().IsSame(&spatial_reference);

    bool is_geo_transform = true;
    for (int i = 0; i < 6; i++)
    {
        if (std::fabs(raster.m_geo_transform[i] - m_geo_transform[i]) > SpatialDriver::FLOAT_BIAS)
        {
            is_geo_transform = false;
            break;
        }
    }

    bool is_same_size = raster.GetXSize() == GetXSize() && raster.GetYSize() == GetYSize();

    return is_same_spatial_reference && is_geo_transform && is_same_size;
}