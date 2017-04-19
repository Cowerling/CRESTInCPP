//
// Created by cowerling on 17-4-19.
//

#ifndef CREST_SPATIALDRIVER_H
#define CREST_SPATIALDRIVER_H

#include <gdal_priv.h>

namespace CREST {
    class SpatialDriver {
    public:
        static void GetCoordinates(GDALDataset *dataset, int x, int y, double &coordinate_x, double &coordinate_y);
    };
}

#endif //CREST_SPATIALDRIVER_H
