//
// Created by cowerling on 17-4-18.
//

#include "Tier.h"

using namespace CREST;

Tier::Tier()
{
    m_next_tier = m_first_destination_tier = m_last_destination_tier = nullptr;
    m_flow_time = m_water_access_ratio = 0;
}

Tier::~Tier()
{}

float Tier::GetExcessPrecipitation() const
{
    return m_excess_precipitation
}

void Tier::SetExcessPrecipitation(float excess_precipitation)
{
    m_excess_precipitation = excess_precipitation;
}