//
// Created by cowerling on 17-4-18.
//
#include <cmath>

#include "Cell.h"

using namespace CREST;

Cell::Cell() : m_flow_time(-1), m_next_cell(nullptr),
               m_soil_water(0),
               m_runoff(0),
               m_actual_evaporation(0)
{
    m_x = m_y = -1;
}

Cell::~Cell()
{}

void Cell::RunoffGenerationProcess(float precipitation, float potential_evaporation, float max_water_capacity, float evaporation_multiplier, float impervious_ratio,
                                   float exponent, float hydra_conductivity)
{
    double evaporation = potential_evaporation * evaporation_multiplier, runoff = 0, new_soil_water = 0;

    if (precipitation > evaporation)
    {
        float precipitation_to_soil = (precipitation - evaporation) * (1 - impervious_ratio);

        if (m_soil_water < max_water_capacity)
        {
            double basin_max_water_capacity = max_water_capacity * (1 + exponent);
            double water_capacity = basin_max_water_capacity * (1 - std::pow(1 - m_soil_water / max_water_capacity, 1 / (1 + exponent)));
            if (precipitation_to_soil + water_capacity > max_water_capacity)
            {
                runoff = precipitation_to_soil - (max_water_capacity - m_soil_water);
                new_soil_water = max_water_capacity;
            }
            else
            {
                runoff = precipitation_to_soil - max_water_capacity * (std::pow(1 - water_capacity / basin_max_water_capacity, 1 + exponent) - std::pow(1 - (water_capacity + precipitation_to_soil) / basin_max_water_capacity, 1 + exponent));
                if (runoff < 0) runoff = 0;
                new_soil_water = m_soil_water + precipitation_to_soil - runoff;
            }
        }
        else
            runoff = precipitation_to_soil;

        float infiltrate_water = (m_soil_water + new_soil_water) / max_water_capacity / 2 * hydra_conductivity;

        if (runoff <= infiltrate_water)
            m_ground_tier.SetExcessPrecipitation(runoff);
        else
            m_ground_tier.SetExcessPrecipitation(infiltrate_water);

        m_surface_tier.SetExcessPrecipitation(runoff - m_ground_tier.GetExcessPrecipitation() + (precipitation - evaporation) * impervious_ratio);
        m_actual_evaporation = evaporation;
    }
    else
    {
        m_surface_tier.SetExcessPrecipitation(0);
        m_ground_tier.SetExcessPrecipitation(0);

        float infiltrate_water = (evaporation - precipitation) * m_soil_water / max_water_capacity;

        if (infiltrate_water < m_soil_water)
            new_soil_water = m_soil_water - infiltrate_water;
        else
            new_soil_water = 0;

        m_actual_evaporation = m_soil_water - new_soil_water;
    }

    m_soil_water = new_soil_water;
}

void Cell::SetX(int x)
{
    m_x = x;
}

void Cell::SetY(int y)
{
    m_y = y;
}

void Cell::SetFlowTime(float flow_time)
{
    m_flow_time = flow_time;
}

void Cell::SetNextCell(Cell *next_cell)
{
    m_next_cell = next_cell;
}

void Cell::SetSoilWater(float soil_water)
{
    m_soil_water = soil_water;
}

void Cell::SetRunoff(float runoff)
{
    m_runoff = runoff;
}