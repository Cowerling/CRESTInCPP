//
// Created by cowerling on 17-4-19.
//

#ifndef CREST_CORE_H
#define CREST_CORE_H

#include "../Base/Basin.h"
#include "../Base/RasterCollection.h"

namespace CREST {
    typedef void (*ProgressReport)(const std::string&, int);

    class Core {
    public:
        Core(Raster *dem, Raster *ddm, Raster *fam, ProgressReport progress_report = nullptr);

    public:
        void Caculate(RasterCollection &precipitations, RasterCollection &potential_evaporations, float time_interval);
        void GetResult(GDALDataset *dataset, StatusResultType type);

    private:
        void SingleCaculate(Raster *precipitation, Raster *potential_evaporation, float time_interval);

    protected:
        Basin *m_basin;
        ProgressReport m_progress_report;
    };
}

#endif //CREST_CORE_H
