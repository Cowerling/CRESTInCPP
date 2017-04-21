//
// Created by cowerling on 17-4-18.
//

#include "Tier.h"

using namespace CREST;

Tier::Tier() : m_destination_tier(nullptr),
               m_reservoir(0),
               m_water_access_ratio(0),
               m_excess_precipitation(0)
{
}

Tier::~Tier()
{}

void Tier::SetExcessPrecipitation(float excess_precipitation)
{
    m_excess_precipitation = excess_precipitation;
}

void Tier::SetWaterAccessRatio(float water_access_ratio)
{
    m_water_access_ratio = water_access_ratio;
}

void Tier::SetDestinationTier(Tier *tier)
{
    m_destination_tier = tier;
}

void Tier::SetReservoir(float reservoir)
{
    m_reservoir = reservoir;
}

void Tier::AdjustDestinationReservoir(float discharge)
{
    if (m_destination_tier != nullptr)
    {
        float reservoir = m_destination_tier->GetReservoir() + discharge * m_water_access_ratio;
        m_destination_tier->SetReservoir(reservoir);

        if (m_destination_tier->GetDestinationTier() != nullptr)
        {
            Tier *next_destination_tier = m_destination_tier->GetDestinationTier();
            reservoir = next_destination_tier->GetReservoir() + discharge * (1 - m_water_access_ratio);
            next_destination_tier->SetReservoir(reservoir);
        }
    }
}