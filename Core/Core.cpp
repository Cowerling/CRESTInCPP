//
// Created by cowerling on 17-4-19.
//

#include "Core.h"
#include "../Spatial/SpatialDriver.h"

using namespace CREST;

Core::Core(Raster *dem, Raster *ddm, Raster *fam) : m_basin(nullptr)
{
    m_basin = new Basin(dem);
    m_basin->SetFlowDirection(ddm, fam);
}

void Core::SingleCaculate(Raster *precipitation, Raster *potential_evaporation, float time_interval)
{
    if (m_basin == nullptr) throw std::runtime_error("lack basin");
    if (precipitation == nullptr) throw std::runtime_error("precipitation is null");
    if (potential_evaporation == nullptr) throw std::runtime_error("potential evaporation is null");

    OGRSpatialReference spatial_reference = m_basin->m_dem->GetSpatialReference(),
            p_spatial_reference = precipitation->GetSpatialReference(),
            pe_spatial_reference = potential_evaporation->GetSpatialReference();

    bool is_same_spatial_reference_p = spatial_reference.IsSame(&p_spatial_reference),
            is_same_spatial_reference_pe = spatial_reference.IsSame(&pe_spatial_reference);

    for (int x = 0; x < m_basin->GetXSize(); x++)
    {
        for (int y = 0; y < m_basin->GetYSize(); y++)
        {
            if (!m_basin->IsValid(x, y)) continue;

            double precipitation_value, potential_evaporation_value;

            if (is_same_spatial_reference_p)
                precipitation_value = precipitation->FindValue(x, y) * time_interval;
            else
            {
                double coordinate_x, coordinate_y;
                m_basin->m_dem->GetCoordinates(x, y, coordinate_x, coordinate_y);

                precipitation_value = precipitation->SearchValue(coordinate_x, coordinate_y) * time_interval;
            }

            if (is_same_spatial_reference_pe)
                potential_evaporation->FindValue(x, y) * time_interval;
            else
            {
                double coordinate_x, coordinate_y;
                m_basin->m_dem->GetCoordinates(x, y, coordinate_x, coordinate_y);

                potential_evaporation_value = potential_evaporation->SearchValue(coordinate_x, coordinate_y) * time_interval;
            }

            if (precipitation_value < 0) precipitation_value = 0;
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

            m_basin->FindCellFlowRoute(x, y, time_interval);

            surface_tier.AdjustDestinationReservoir(surface_discharge);
            ground_tier.AdjustDestinationReservoir(ground_discharge);
        }
    }
}

void Core::Caculate(RasterCollection &precipitations, RasterCollection &potential_evaporations, float time_interval)
{
    if (precipitations.Size() != potential_evaporations.Size())
        throw std::runtime_error("precipitation and potential evaporation size not match");

    for (int i = 0; i < precipitations.Size(); i++)
    {
        SingleCaculate(precipitations[i], potential_evaporations[i], time_interval);
    }
}

void Core::GetResult(GDALDataset *dataset, StatusResultType type)
{
    double geo_transform[6];
    for (int i = 0; i < 6; i++)
        geo_transform[i] = m_basin->m_dem->GetGeotransform()[i];

    char *wkt = nullptr;
    m_basin->m_dem->GetSpatialReference().exportToWkt(&wkt);

    float *status_results = m_basin->GetStatusResults(type);

    dataset->SetGeoTransform(geo_transform);
    dataset->SetProjection(wkt);

    dataset->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, m_basin->GetXSize(), m_basin->GetYSize(), status_results, m_basin->GetXSize(), m_basin->GetYSize(), GDT_Float32, 0, 0);

    GDALClose(dataset);
    delete[] status_results;
    delete[] wkt;
}