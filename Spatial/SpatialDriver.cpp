//
// Created by cowerling on 17-4-19.
//

#include "SpatialDriver.h"

using namespace CREST;

void SpatialDriver::GetCoordinates(GDALDataset *dataset, int x, int y, double &coordinate_x,
                                   double &coordinate_y)
{
    double geo_transform[6];
    dataset->GetGeoTransform(geo_transform);
    int y_size = dataset->GetRasterYSize();

    coordinate_x = geo_transform[0] + geo_transform[1] * x + geo_transform[2] * (y_size - y);
    coordinate_y = geo_transform[3] + geo_transform[4] * x + geo_transform[5] * (y_size - y - 1);
}