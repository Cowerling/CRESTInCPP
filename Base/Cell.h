//
// Created by cowerling on 17-4-18.
//

#ifndef CREST_CELL_H
#define CREST_CELL_H

#include "Tier.h"
#include "StatusResultType.h"

namespace CREST {
    class Cell {
    public:
        Cell();
        virtual ~Cell();

        void RunoffGenerationProcess(float precipitation, float potential_evaporation, float max_water_capacity, float evaporation_multiplier, float impervious_ratio,
                                     float exponent, float hydra_conductivity);
        inline int GetX() const;
        void SetX(int x);
        inline int GetY() const;
        void SetY(int y);
        inline float GetFlowTime() const;
        void SetFlowTime(float flow_time);
        inline const Cell* GetNextCell() const;
        inline Cell* GetNextCell();
        void SetNextCell(Cell *next_cell);
        inline Tier& GetSurfaceTier();
        inline Tier& GetGroundTier();
        inline float GetSoilWater() const;
        void SetSoilWater(float soil_water);
        inline float GetRunoff() const;
        void SetRunoff(float runoff);
        float GetStatusResult(StatusResultType type) const;

    protected:
        Tier m_surface_tier, m_ground_tier;
        float m_actual_evaporation;
        float m_soil_water; //refers to the initial value of soil water.
        int m_x, m_y;
        float m_flow_time;
        float m_runoff;

        Cell *m_next_cell;
    };

    inline int Cell::GetX() const
    {
        return m_x;
    }

    inline int Cell::GetY() const
    {
        return m_y;
    }

    inline float Cell::GetFlowTime() const
    {
        return m_flow_time;
    }

    inline const Cell* Cell::GetNextCell() const
    {
        return m_next_cell;
    }

    inline Cell* Cell::GetNextCell()
    {
        return m_next_cell;
    }

    inline Tier& Cell::GetSurfaceTier()
    {
        return m_surface_tier;
    }

    inline Tier& Cell::GetGroundTier()
    {
        return m_ground_tier;
    }

    inline float Cell::GetSoilWater() const {
        return m_soil_water;
    }

    inline float Cell::GetRunoff() const
    {
        return m_runoff;
    }
}

#endif //CREST_CELL_H
