//
// Created by cowerling on 17-4-19.
//

#ifndef CREST_ACTUALRASTER_H
#define CREST_ACTUALRASTER_H

#include "Raster.h"
#include "../Spatial/SpatialDriver.h"

namespace CREST {
    template<class T> class ActualRaster : public Raster {
    public:
        ActualRaster(GDALDataset *dataset, GDALDataType data_type);
        virtual ~ActualRaster();

        virtual double FindValue(int x, int y);
        virtual double SearchValue(double coordinate_x, double coordinate_y);
        virtual bool IsNoData(int x, int y);

    protected:
        T FindOriginalValue(int x, int y);

    protected:
        T *m_data;
    };

    template<class T> ActualRaster::ActualRaster(GDALDataset *dataset, GDALDataType data_type) : Raster(dataset, data_type), m_data(nullptr)
    {}

    template<class T> virtual ActualRaster::~ActualRaster()
    {
        delete[] m_data;
    }

    template<class T> T ActualRaster::FindOriginalValue(int x, int y)
    {
        if (x < 0 || x >= m_dataset->GetRasterXSize() || y < 0 || y > m_dataset->GetRasterYSize())
        throw std::runtime_error("out of extent");

        if (m_data == nullptr || !this->Contain(x, y))
        {
            m_origin_x = x - m_x_extent;
            if (m_origin_x < 0) m_origin_x = 0;

            m_origin_y = y - m_y_extent;
            if (m_origin_y < 0) m_origin_y = 0;

            m_x_search_size = m_origin_x + 2 * m_x_extent + 1;
            m_y_search_size = m_origin_y + 2 * m_y_extent + 1;
            if (m_x_search_size > m_x_size) m_x_search_size = m_x_size;
            if (m_y_search_size > m_y_size) m_y_search_size = m_y_size;

            int x_offset = m_origin_x, y_offset = m_y_size - 1 - m_origin_y - m_y_search_size;

            delete[] m_data;
            m_data = new T[m_x_search_size * m_y_search_size];

            m_dataset->GetRasterBand(1)->RasterIO(GF_Read, x_offset, y_offset, m_x_search_size, m_y_search_size, m_data, m_x_search_size, m_y_search_size, m_data_type, 0, 0);
        }

        int relative_x = x - m_origin_x, relative_y = m_y_search_size - 1 - (y - m_origin_y);
        return m_data[relative_y * m_x_search_size + relative_x];
    }

    template<class T> virtual double ActualRaster::FindValue(int x, int y)
    {
        return (double)FindOriginalValue(x, y);
    }

    template<class T> virtual double ActualRaster::SearchValue(double coordinate_x, double coordinate_y)
    {
        int x, y;
        SpatialDriver::GetXYIndex(m_geo_transform, m_y_size, coordinate_x, coordinate_y, x, y);

        return FindValue(x, y);
    }

    template<class T> virtual bool ActualRaster::IsNoData(int x, int y)
    {
        if (std::abs(FindOriginalValue(x, y) - m_no_data_value) < SpatialDriver::FLOAT_BIAS)
            return true;
        else
            return false;
    }
}

#endif //CREST_ACTUALRASTER_H
