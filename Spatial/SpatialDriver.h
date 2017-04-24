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
        const static char *DEFAULT_EPSG = "EPSG:4326";
        static const double EARTH_RADIUS = 6367000;
        static const double PI = 3.14159265359;
        static const double FLOAT_BIAS = 1e-6;
        static const int HOUR_TO_SECOND = 3600;
        static const double MIN_DOUBLE = std::numeric_limits<double>::min();
    };
}

#endif //CREST_SPATIALDRIVER_H
