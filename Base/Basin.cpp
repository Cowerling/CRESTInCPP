//
// Created by cowerling on 17-4-19.
//
#include <cstring>

#include "Basin.h"
#include "../Spatial/SpatialDriver.h"

using namespace CREST;

Basin::Basin(Raster *dem) : m_cells(nullptr), m_x_resolution(0), m_y_resolution(0), m_dem(dem),
                            m_dem_correction(1.259213)
{
    if (m_dem == nullptr) return;

    m_cells = new Cell*[GetYSize()];
    for (int i = 0; i < GetYSize(); i++)
        m_cells[i] = new Cell[GetXSize()];

    const double *geo_transform = m_dem->GetGeotransform();
    OGRSpatialReference spatial_reference = m_dem->GetSpatialReference();

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

Basin::~Basin()
{
    for (int i = 0; i < GetYSize(); ++i)
        delete[] m_cells[i];
    delete[] m_cells;

    delete m_dem;
    delete m_ddm;
    delete m_fam;
}

int Basin::GetXSize() const
{
    return m_dem->GetXSize();
}

int Basin::GetYSize() const
{
    return m_dem->GetYSize();
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

void Basin::SetFlowDirection(Raster *ddm, Raster *fam)
{
    if (ddm->GetXSize() != GetXSize() || ddm->GetYSize() != GetYSize() || fam->GetXSize() || GetXSize() || fam->GetYSize() != GetYSize())
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

    int next_x, next_y;
    double length = 0;
    switch ((int)m_ddm->FindValue(cell.GetX(), cell.GetY()))
    {
        case 1:
            next_x = cell.GetX() + 1;
            next_y = cell.GetY();
            if (cell.GetX() + 1 < GetXSize()) length = m_x_resolution;
            break;
        case 2:
            if (cell.GetX() + 1 < GetXSize() && cell.GetY() - 1 >= 0) length = std::sqrt(m_x_resolution * m_x_resolution + m_y_resolution + m_y_resolution);
            break;
        case 4:
            if (cell.GetY() - 1 >= 0) length = m_y_resolution;
            break;
        case 8:
            if (cell.GetX() - 1 >= 0 && cell.GetY() - 1 >= 0) length = std::sqrt(m_x_resolution * m_x_resolution + m_y_resolution + m_y_resolution);
            break;
        case 16:
            if (cell.GetX() - 1 >= 0) length = m_x_resolution;
            break;
        case 32:
            if (cell.GetX() - 1 >= 0 && cell.GetY() + 1 < GetYSize()) length = std::sqrt(m_x_resolution * m_x_resolution + m_y_resolution + m_y_resolution);
            break;
        case 64:
            if (cell.GetY() + 1 < GetYSize()) length = m_y_resolution;
            break;
        case 128:
            if (cell.GetX() + 1 < GetXSize() && cell.GetY() + 1 < GetYSize()) length = std::sqrt(m_x_resolution * m_x_resolution + m_y_resolution + m_y_resolution);
            break;
        default:
            break;
    }

    if (m_dem->IsNoData(cell.GetX(), cell.GetY())) length = 0;


}

void Basin::SetDEMCorrection(double dem_correction)
{
    m_dem_correction = dem_correction;
}