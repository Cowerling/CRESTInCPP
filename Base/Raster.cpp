//
// Created by cowerling on 17-4-19.
//

#include "Raster.h"
#include "ActualRaster.h"

using namespace CREST;

Raster::Raster()
{}

Raster::Raster(GDALDataset *dataset) : m_dataset(dataset), m_x_extent(0), m_y_extent(0)
{
}

Raster::~Raster()
{
    GDALClose(m_dataset);
}

Raster* Raster::CreateRaster(GDALDataset *dataset)
{
    GDALDataType data_type = dataset->GetRasterBand(1)->GetRasterDataType();

    switch (data_type)
    {
        case GDT_Byte:
            return new ActualRaster<char>(dataset);
        case GDT_UInt16:
            return new ActualRaster<unsigned short>(dataset);
        case GDT_Int16:
            return new ActualRaster<short>(dataset);
        case GDT_UInt32:
            return new ActualRaster<unsigned int>(dataset);
        case GDT_Int32:
            return new ActualRaster<int>(dataset);
        case GDT_Float32:
            return new ActualRaster<float>(dataset);
        case GDT_Float64:
        default:
            return new ActualRaster<double>(dataset);
    }
}