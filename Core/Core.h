//
// Created by cowerling on 17-4-19.
//

#ifndef CREST_CORE_H
#define CREST_CORE_H

#include "../Base/Basin.h"

namespace CREST {
    class Core {
    public:
        Core(Raster *dem, Raster *ddm, Raster *fam);

    private:
        void SingleCaculate(Raster *precipitation, Raster *evaporation, float time_interval);

    protected:
        Basin *m_basin;
    };
}

#endif //CREST_CORE_H
