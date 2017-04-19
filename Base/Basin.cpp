//
// Created by cowerling on 17-4-19.
//
#include <cstring>

#include "Basin.h"
#include "../Spatial/SpatialDriver.h"

using namespace CREST;

Basin::Basin(GDALDataset *dem) : m_cells(nullptr), m_x_resolution(0), m_y_resolution(0), m_dem(dem)
{
    if (m_dem == nullptr) return;

    m_cells = new Cell*[GetYSize()];
    for (int i = 0; i < GetYSize(); i++)
        m_cells[i] = new Cell[GetXSize()];

    double geo_transform[6];
    if(m_dem->GetGeoTransform(geo_transform) == CE_None)
    {
        const char *projectionRef = m_dem->GetProjectionRef();
        char *wkt = new char[std::strlen(projectionRef)];
        std::strcpy(wkt, projectionRef);
        OGRSpatialReference spatial_reference;
        spatial_reference.importFromWkt(&wkt);    //it seems can not delete wkt

        if (spatial_reference.IsProjected())
        {
            m_x_resolution = geo_transform[1];
            m_y_resolution = -geo_transform[5];
        }
        else if (spatial_reference.IsGeographic())
        {
            m_x_resolution = geo_transform[1] / 180 * SpatialDriver::PI * SpatialDriver::EARTH_RADIUS;
            m_y_resolution = -geo_transform[5] / 180 * SpatialDriver::PI * SpatialDriver::EARTH_RADIUS;
        }
        else
        {
            if (-180 <= geo_transform[0] && geo_transform[0] <= 180 && -90 <= geo_transform[3] && geo_transform[3] <= 90)
            {
                m_x_resolution = geo_transform[1] / 180 * SpatialDriver::PI * SpatialDriver::EARTH_RADIUS;
                m_y_resolution = -geo_transform[5] / 180 * SpatialDriver::PI * SpatialDriver::EARTH_RADIUS;
            }
            else
            {
                m_x_resolution = geo_transform[1];
                m_y_resolution = -geo_transform[5];
            }
        }
    }


}

Basin::~Basin()
{
    for (int i = 0; i < GetYSize(); ++i)
        delete[] m_cells[i];
    delete[] m_cells;

    GDALClose(m_dem);
    GDALClose(m_ddm);
    GDALClose(m_fam);
}

int Basin::GetXSize() const
{
    return m_dem->GetRasterXSize();
}

int Basin::GetYSize() const
{
    return m_dem->GetRasterYSize();
}

const Cell& Basin::operator()(int x, int y) const
{
    return (*this)(x, y);
}

Cell& Basin::operator()(int x, int y)
{
    Cell &cell = m_cells[GetYSize() - 1 - y][x];
    cell.SetX(x);
    cell.SetY(y);

    return cell;
}

void Basin::SetFlowDirection(GDALDataset *ddm, GDALDataset *fam)
{
    if (ddm->GetRasterXSize() != GetXSize() || ddm->GetRasterYSize() != GetYSize() || fam->GetRasterXSize() || GetXSize() || fam->GetRasterYSize() != GetYSize())
        throw std::runtime_error("size not match dem");

    m_ddm = ddm;
    m_fam = fam;
}

void Basin::FindCellFlowRoute(int x, int y)
{

}

void Basin::FindCellFlowRoute(Cell &cell)
{

}

void Basin::CaculateFlowTime(Cell &cell)
{
    if (cell.GetX() == -1 || cell.GetY() == -1)
        throw std::runtime_error("cell lack x or y");


}