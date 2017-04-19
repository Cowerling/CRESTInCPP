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

    protected:
        Tier *m_next_tier;
        Tier *m_first_destination_tier, *m_last_destination_tier;
        float m_flow_time;
        float m_water_access_ratio;
        float m_excess_precipitation;
        float m_reservoir;
    };

    inline float Tier::GetExcessPrecipitation() const
    {
        return m_excess_precipitation;
    }
}

#endif //CREST_TIER_H
