//
// Created by cowerling on 17-4-19.
//

#ifndef CREST_BASIN_H
#define CREST_BASIN_H

#include <gdal_priv.h>
#include <ogrsf_frmts.h>

#include "Cell.h"
#include "Raster.h"

namespace CREST {
    class Basin {
    public:
        Basin(Raster *dem);
        ~Basin();

        int GetXSize() const;
        int GetYSize() const;
        bool IsValid(int x, int y);
        inline double GetXResolution() const;
        inline double GetYResolution() const;
        const Cell& operator()(int x, int y) const;
        Cell& operator()(int x, int y);
        void SetFlowDirection(Raster *ddm, Raster *fam);
        void FindCellFlowRoute(int x, int y, float time_interval);

        void SetDEMCorrection(double dem_correction);
        inline double GetMaxSoilWater() const;
        void SetMaxSoilWater(double max_soil_water);
        inline double GetEvaporationMultiplier() const;
        void SetEvaporationMultiplier(double evaporation_multiplier);
        inline double GetImperviousRatio() const;
        void SetImperviousRatio(double impervious_ratio);
        inline double GetExponent() const;
        void SetExponent(double exponent);
        inline double GetHydraConductivity() const;
        void SetHydraConductivity(double hydra_conductivity);
        inline double GetSurfaceReservoirDischarge() const;
        void SetSurfaceReservoirDischarge(double surface_reservoir_discharge);
        inline double GetGroundReservoirDischarge() const;
        void SetGroundReservoirDischarge(double ground_reservoir_discharge);
        inline double GetInitialSoilWater() const;
        void SetInitialSoilWater(double initial_soil_water);

        float* GetStatusResults(StatusResultType type) const;

    private:
        void FindCellFlowRoute(Cell &cell, float time_interval);
        void CaculateFlowTime(Cell &cell);

    protected:
        Cell **m_cells;
        Raster *m_dem, *m_ddm, *m_fam;
        double m_x_resolution, m_y_resolution;

        double m_dem_correction;    //refers to the change in DEM used for calculating the slope when the DEM for the downstream cell is higher than the upstream or the downstream cell is a nodata/outside region cell.
        double m_vegetation_speed_factor;
        double m_slope_speed_multiplier;    //refers to the slope flow speed multiplier.
        double m_slope_speed_exponent;  //refers to the slope flow speed exponent.
        double m_channel_accumulation;  //refers to the flow accumulation needed for a cell to be marked as a channel cell.
        double m_channel_speed_multiplier;  //refers to the multiplier used to convert slope flow speed to channel flow speed.
        double m_interflow_speed_multiplier;    //refers to the multiplier used to convert slope flow speed to interflow flow speed.
        double m_max_soil_water; //refers to the maximal soil water capacity (in depth) of three soil layers.
        double m_evaporation_multiplier;    //refers to the multiplier to convert between input PET and local actual ET.
        double m_impervious_ratio;  //refers to the Impervious area ratio.
        double m_exponent;  //refers to the exponent of the variable infiltration curve.
        double m_hydra_conductivity;    //refers to the soil saturated hydraulic conductivity.
        double m_surface_reservoir_discharge;    //refers to the overland reservoir discharge multiplier.
        double m_ground_reservoir_discharge; //refers to the interflow reservoir discharge multiplier.
        double m_initial_soil_water;    //refers to the initial value of soil water.refers to the initial value of soil water.

        bool m_flow_route_intialize;

    public:
        friend class Core;
    };

    inline double Basin::GetXResolution() const
    {
        return m_x_resolution;
    }

    inline double Basin::GetYResolution() const
    {
        return m_y_resolution;
    }

    inline double Basin::GetMaxSoilWater() const
    {
        return m_max_soil_water;
    }

    inline double Basin::GetEvaporationMultiplier() const
    {
        return m_evaporation_multiplier;
    }

    inline double Basin::GetImperviousRatio() const
    {
        return m_impervious_ratio;
    }

    inline double Basin::GetExponent() const
    {
        return m_exponent;
    }

    inline double Basin::GetHydraConductivity() const
    {
        return m_hydra_conductivity;
    }

    inline double Basin::GetSurfaceReservoirDischarge() const
    {
        return m_surface_reservoir_discharge;
    }

    inline double Basin::GetGroundReservoirDischarge() const
    {
        return m_ground_reservoir_discharge;
    }

    inline double Basin::GetInitialSoilWater() const
    {
        return m_initial_soil_water;
    }
}

#endif //CREST_BASIN_H
