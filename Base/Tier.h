//
// Created by cowerling on 17-4-18.
//

#ifndef CREST_TIER_H
#define CREST_TIER_H

namespace CREST {
    class Tier {
    public:
        Tier();
        virtual ~Tier();

        inline float GetExcessPrecipitation() const;
        void SetExcessPrecipitation(float excess_precipitation);
        inline float GetWaterAccessRatio() const;
        void SetWaterAccessRatio(float water_access_ratio);
        inline Tier* GetDestinationTier();
        void SetDestinationTier(Tier *tier);
        inline float GetReservoir() const;
        void SetReservoir(float reservoir);
        void AdjustDestinationReservoir(float discharge);

    protected:
        Tier *m_destination_tier;
        float m_water_access_ratio;
        float m_excess_precipitation;
        float m_reservoir;
    };

    inline float Tier::GetExcessPrecipitation() const
    {
        return m_excess_precipitation;
    }

    inline float Tier::GetWaterAccessRatio() const
    {
        return m_water_access_ratio;
    }

    inline Tier* Tier::GetDestinationTier()
    {
        return m_destination_tier;
    }

    inline float Tier::GetReservoir() const
    {
        return m_reservoir;
    }
}

#endif //CREST_TIER_H
