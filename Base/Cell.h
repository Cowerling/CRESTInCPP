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
    protected:
        Tier m_surface_tier, m_ground_tier;
        float m_precipitation;
        float m_potential_evaporation, m_actual_evaporation;
        float m_soil_water; //refers to the initial value of soil water. This is a percentage of the pWm and should therefore vary between 0 and 100.
    };
}

#endif //CREST_CELL_H
