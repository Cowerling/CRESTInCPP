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
                            m_channel_speed_multiplier(1.129872),
                            m_max_soil_water(337.073669),
                            m_evaporation_multiplier(0.387291),
                            m_impervious_ratio(8.852914),
                            m_exponent(0.961943),
                            m_hydra_conductivity(6.506332),
                            m_surface_reservoir_discharge(0.357427),
                            m_ground_reservoir_discharge(0.119561),
                            m_initial_soil_water(0.63829159)
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
    if (cell.GetX() < 0 || cell.GetY() < 0)
    {
        cell.SetX(x);
        cell.SetY(y);
        cell.SetSoilWater(m_max_soil_water * m_initial_soil_water);
    }

    return cell;
}

void Basin::SetFlowDirection(Raster *ddm, Raster *fam)
{
    if (ddm->GetXSize() != GetXSize() || ddm->GetYSize() != GetYSize() || fam->GetXSize() || GetXSize() || fam->GetYSize() != GetYSize())
        throw std::runtime_error("size not match dem");

    m_ddm = ddm;
    m_fam = fam;
}

void Basin::FindCellFlowRoute(int x, int y, float time_interval)
{
    FindCellFlowRoute(this->operator()(x, y), time_interval);
}

void Basin::FindCellFlowRoute(Cell &cell, float time_interval)
{
    if (cell.GetFlowTime() < 0)
        CaculateFlowTime(cell);

    Cell *next_cell = &cell;
    float pre_surface_flow_time = 0, surface_flow_time = 0, pre_ground_flow_time = 0, ground_flow_time = 0;

    while (next_cell != nullptr && (surface_flow_time < time_interval || ground_flow_time < time_interval))
    {
        if (next_cell->GetFlowTime() < 0) CaculateFlowTime(*next_cell);

        if (surface_flow_time < time_interval)
        {
            pre_surface_flow_time = surface_flow_time;
            surface_flow_time += next_cell->GetFlowTime();

            cell.GetSurfaceTier().SetDestinationTier(&next_cell->GetSurfaceTier());
        }

        if (ground_flow_time < time_interval)
        {
            pre_ground_flow_time = ground_flow_time;
            ground_flow_time += next_cell->GetFlowTime() / m_interflow_speed_multiplier;

            cell.GetGroundTier().SetDestinationTier(&next_cell->GetSurfaceTier());
        }

        next_cell = next_cell->GetNextCell();
    }

    if (surface_flow_time > time_interval)
        cell.GetSurfaceTier().SetWaterAccessRatio(1 - (time_interval - pre_surface_flow_time) / (surface_flow_time - pre_surface_flow_time));
    else
        cell.GetSurfaceTier().SetWaterAccessRatio(0);

    if (ground_flow_time > time_interval)
        cell.GetGroundTier().SetWaterAccessRatio(1 - (time_interval - pre_ground_flow_time) / (ground_flow_time - pre_ground_flow_time));
    else
        cell.GetGroundTier().SetWaterAccessRatio(0);
}

void Basin::CaculateFlowTime(Cell &cell)
{
    int next_x = -1, next_y = -1;
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

    if (0 <= next_x && next_x < GetXSize() && 0 <= next_y && next_y < GetYSize() && !m_dem->IsNoData(next_x, next_y))
        cell.SetNextCell(&this->operator()(next_x, next_y));

    double slope = m_dem_correction / m_y_resolution;
    if (cell.GetNextCell() != nullptr && length != 0)
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

bool Basin::IsValid(int x, int y)
{
    if (!m_dem->IsNoData(x, y)) return true;
    else return false;
}

void Basin::SetMaxSoilWater(double max_soil_water)
{
    m_max_soil_water = max_soil_water;
}

void Basin::SetEvaporationMultiplier(double evaporation_multiplier)
{
    m_evaporation_multiplier = evaporation_multiplier;
}

void Basin::SetImperviousRatio(double impervious_ratio)
{
    m_impervious_ratio = impervious_ratio;
}

void Basin::SetExponent(double exponent)
{
    m_exponent = exponent;
}

void Basin::SetHydraConductivity(double hydra_conductivity)
{
    m_hydra_conductivity = hydra_conductivity;
}

void Basin::SetSurfaceReservoirDischarge(double surface_reservoir_discharge)
{
    m_surface_reservoir_discharge = surface_reservoir_discharge;
}

void Basin::SetGroundReservoirDischarge(double ground_reservoir_discharge)
{
    m_ground_reservoir_discharge = ground_reservoir_discharge;
}

void Basin::SetInitialSoilWater(double initial_soil_water)
{
    m_initial_soil_water = initial_soil_water;
}