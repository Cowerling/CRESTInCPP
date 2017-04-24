//
// Created by cowerling on 17-4-19.
//

#ifndef CREST_RASTER_H
#define CREST_RASTER_H

#include <gdal_priv.h>
#include <ogrsf_frmts.h>

namespace CREST {
    class Raster {
    public:
        virtual ~Raster();

        static Raster* CreateRaster(GDALDataset *dataset, int x_extent = 200, int y_extent = 200);
        virtual double FindValue(int x, int y) const = 0;
        virtual double SearchValue(double coordinate_x, double coordinate_y) const = 0;
        void SetExtent(int x_extent, int y_extent);
        inline int GetXSize() const;
        inline int GetYSize() const;
        inline const double* GetGeotransform() const;
        OGRSpatialReference GetSpatialReference() const;
        virtual bool IsNoData(int x, int y) = 0;
        void GetCoordinates(int x, int y, double &coordinate_x, double &coordinate_y);

    protected:
        Raster(GDALDataset *dataset, GDALDataType data_type);
        bool Contain(int x, int y) const;

    private:
        Raster();

    protected:
        GDALDataset *m_dataset;
        GDALDataType m_data_type;
        int m_origin_x, m_origin_y;
        int m_x_extent, m_y_extent;
        int m_x_search_size, m_y_search_size;
        int m_x_size, m_y_size;
        double m_geo_transform[6];
        double m_no_data_value;
    };

    inline int Raster::GetXSize() const
    {
        return m_x_size;
    }

    inline int Raster::GetYSize() const
    {
        return m_y_size;
    }

    inline const double* Raster::GetGeotransform() const
    {
        return m_geo_transform;
    }
};

#endif //CREST_RASTER_H
