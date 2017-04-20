//
// Created by cowerling on 17-4-18.
//

#ifndef CREST_CELL_H
#define CREST_CELL_H

#include "Tier.h"

namespace CREST {
    class Cell {
    public:
        Cell();
        virtual ~Cell();

        void RunoffGenerationProcess(float max_water_capacity, float evaporation_multiplier, float impervious_ratio,
                                     float exponent, float hydra_conductivity);
        inline int GetX() const;
        void SetX(int x);
        inline int GetY() const;
        void SetY(int y);
        inline float GetFlowTime() const;
        void SetFlowTime(float flow_time);

    protected:
        Tier m_surface_tier, m_ground_tier;
        float m_precipitation;
        float m_potential_evaporation, m_actual_evaporation;
        float m_soil_water; //refers to the initial value of soil water. This is a percentage of the pWm and should therefore vary between 0 and 100.
        int m_x, m_y;
        float m_flow_time;
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
}

#endif //CREST_CELL_H
