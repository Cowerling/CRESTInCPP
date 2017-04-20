//
// Created by cowerling on 17-4-19.
//
#include <cstring>
#include <cmath>

#include "Basin.h"
#include "../Spatial/SpatialDriver.h"

using namespace CREST;

Basin::Basin(Raster *dem) : m_cells(nullptr), m_x_resolution(0), m_y_resolution(0), m_dem(dem),
                            m_dem_correction(1.259213),
                            m_vegetation_speed_factor(1),
                            m_slope_speed_multiplier(107.67186),
                            m_slope_speed_exponent(0.30501),
                            m_channel_accumulation(54.522789),
                            m_channel_speed_multiplier(1.129872)
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
            length = m_x_resolution;
            break;
        case 2:
            next_x = cell.GetX() + 1;
            next_y = cell.GetY() - 1;
            length = std::sqrt(m_x_resolution * m_x_resolution + m_y_resolution + m_y_resolution);
            break;
        case 4:
            next_x = cell.GetX();
            next_y = cell.GetY() - 1;
            length = m_y_resolution;
            break;
        case 8:
            next_x = cell.GetX() - 1;
            next_y = cell.GetY() - 1;
            length = std::sqrt(m_x_resolution * m_x_resolution + m_y_resolution + m_y_resolution);
            break;
        case 16:
            next_x = cell.GetX() - 1;
            next_y = cell.GetY();
            length = m_x_resolution;
            break;
        case 32:
            next_x = cell.GetX() - 1;
            next_y = cell.GetY() + 1;
            length = std::sqrt(m_x_resolution * m_x_resolution + m_y_resolution + m_y_resolution);
            break;
        case 64:
            next_x = cell.GetX();
            next_y = cell.GetY() + 1;
            length = m_y_resolution;
            break;
        case 128:
            next_x = cell.GetX() + 1;
            next_y = cell.GetY() + 1;
            length = std::sqrt(m_x_resolution * m_x_resolution + m_y_resolution + m_y_resolution);
            break;
        default:
            break;
    }

    if (next_x < 0 || next_x >= GetXSize() || next_y < 0 || next_y >= GetYSize() || m_dem->IsNoData(cell.GetX(), cell.GetY())) length = 0;

    double slope = m_dem_correction / m_y_resolution;
    if (length != 0)
        slope = m_dem->FindValue(cell.GetX(), cell.GetY()) > m_dem->FindValue(next_x, next_y) ? (m_dem->FindValue(cell.GetX(), cell.GetY()) - m_dem->FindValue(next_x, next_y)) / length : m_dem_correction / length;

    double speed = m_slope_speed_multiplier * m_vegetation_speed_factor * std::pow(slope, m_slope_speed_exponent);
    if (m_fam->FindValue(cell.GetX(), cell.GetY()) > m_channel_accumulation) speed *= m_channel_speed_multiplier;

    if (length == 0) length = m_y_resolution;

    cell.SetFlowTime(length / speed / SpatialDriver::HOUR_TO_SECOND);
}

void Basin::SetDEMCorrection(double dem_correction)
{
    m_dem_correction = dem_correction;
}