//
// Created by cowerling on 17-4-19.
//

#ifndef CREST_RASTER_H
#define CREST_RASTER_H

#include <gdal_priv.h>

namespace CREST {
    class Raster {
    public:
        virtual ~Raster();

        static Raster* CreateRaster(GDALDataset *dataset);
        virtual double GetValue(int x, int y) const = 0;
        void SetExtent();

    protected:
        Raster(GDALDataset *dataset);

    private:
        Raster();

    protected:
        GDALDataset *m_dataset;
        int m_x_extent, m_y_extent;
    };
};

#endif //CREST_RASTER_H
