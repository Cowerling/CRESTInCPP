//
// Created by cowerling on 17-4-19.
//

#include "Core.h"

using namespace CREST;

Core::Core(Raster *dem, Raster *ddm, Raster *fam) : m_basin(nullptr)
{
    m_basin = new Basin(dem);
    m_basin->SetFlowDirection(ddm, fam);
}

void Core::SingleCaculate(Raster *precipitation, Raster *potential_evaporation, float time_interval)
{
    if (m_basin == nullptr) throw std::runtime_error("lack basin");

    OGRSpatialReference spatial_reference = m_basin->m_dem->GetSpatialReference(),
            precipitation_spatial_reference = precipitation->GetSpatialReference(),
            potential_evaporation_spatial_reference = potential_evaporation->GetSpatialReference();

    bool is_same_spatial_reference_precipitation = spatial_reference.IsSame(&precipitation_spatial_reference),
            is_same_spatial_reference_potential_evaporation = spatial_reference.IsSame(&potential_evaporation_spatial_reference);

    for (int x = 0; x < m_basin->GetXSize(); x++)
    {
        for (int y = 0; y < m_basin->GetYSize(); y++)
        {
            if (!m_basin->IsValid(x, y)) continue;



            double precipitation_value = precipitation->FindValue(x, y) * time_interval;
            if (precipitation_value < 0) precipitation_value = 0;

            double potential_evaporation_value = potential_evaporation->FindValue(x, y) * time_interval;
            if (potential_evaporation_value < 0) potential_evaporation_value = 0;

            Cell &cell = m_basin->operator()(x, y);

            cell.RunoffGenerationProcess(precipitation_value,
                                                   potential_evaporation_value,
                                                   m_basin->GetMaxSoilWater(),
                                                   m_basin->GetEvaporationMultiplier(),
                                                   m_basin->GetImperviousRatio(),
                                                   m_basin->GetExponent(),
                                                   m_basin->GetHydraConductivity());

            Tier &surface_tier = cell.GetSurfaceTier(), ground_tier = cell.GetGroundTier();

            surface_tier.SetReservoir(surface_tier.GetReservoir() + surface_tier.GetExcessPrecipitation());
            ground_tier.SetReservoir(ground_tier.GetReservoir() + ground_tier.GetExcessPrecipitation());

            float surface_reservoir = surface_tier.GetReservoir() + surface_tier.GetExcessPrecipitation(),
                    ground_reservoir = ground_tier.GetReservoir() + ground_tier.GetExcessPrecipitation();

            float surface_discharge = surface_reservoir * m_basin->GetSurfaceReservoirDischarge(),
                    ground_discharge = ground_reservoir * m_basin->GetGroundReservoirDischarge();

            surface_tier.SetReservoir(surface_reservoir - surface_discharge);
            ground_tier.SetReservoir(ground_reservoir - ground_discharge);

            cell.SetRunoff((surface_discharge + ground_discharge) * m_basin->GetXResolution() * m_basin->GetYResolution() / time_interval / 3.6);

            surface_tier.AdjustDestinationReservoir(surface_discharge);
            ground_tier.AdjustDestinationReservoir(ground_discharge);
        }
    }
}