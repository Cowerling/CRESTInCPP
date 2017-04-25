//
// Created by cowerling on 17-4-19.
//

#ifndef CREST_SPATIALDRIVER_H
#define CREST_SPATIALDRIVER_H

#include <limits>
#include <gdal_priv.h>

namespace CREST {
    class SpatialDriver {
    public:
        static void GetCoordinates(GDALDataset *dataset, int x, int y, double &coordinate_x, double &coordinate_y);
        static void GetCoordinates(const double geo_transform[6], int y_size, int x, int y, double &coordinate_x, double &coordinate_y);
        static void GetXYIndex(const double geo_transform[6], int y_size, double coordinate_x, double coordinate_y, int &x, int &y);

    public:
        static const char *DEFAULT_EPSG;
        static const double EARTH_RADIUS;
        static const double PI;
        static const double FLOAT_BIAS;
        static const int HOUR_TO_SECOND;
        static const double MIN_DOUBLE;
        static const double MIN_FLOAT;
    };
}

#endif //CREST_SPATIALDRIVER_H
