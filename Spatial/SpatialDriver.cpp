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

    coordinate_x = geo_transform[0] + geo_transform[1] * x + geo_transform[2] * (y_size - 1 - y);
    coordinate_y = geo_transform[3] + geo_transform[4] * x + geo_transform[5] * (y_size - 1 - y);
}

void SpatialDriver::GetCoordinates(const double *geo_transform, int y_size, int x, int y, double &coordinate_x,
                                   double &coordinate_y)
{
    coordinate_x = geo_transform[0] + geo_transform[1] * x + geo_transform[2] * (y_size - 1 - y);
    coordinate_y = geo_transform[3] + geo_transform[4] * x + geo_transform[5] * (y_size - 1 - y);
}

void SpatialDriver::GetXYIndex(const double *geo_transform, int y_size, double coordinate_x, double coordinate_y, int &x,
                               int &y)
{
    x = (int)(((coordinate_x - geo_transform[0]) * geo_transform[5] - (coordinate_y - geo_transform[3]) * geo_transform[2]) / (geo_transform[1] * geo_transform[5] - geo_transform[2] * geo_transform[4]));
    y = y_size - 1 - (int)(((coordinate_y - geo_transform[3]) * geo_transform[1] - (coordinate_x - geo_transform[0]) * geo_transform[4]) / (geo_transform[1] * geo_transform[5] - geo_transform[2] * geo_transform[4]));
}