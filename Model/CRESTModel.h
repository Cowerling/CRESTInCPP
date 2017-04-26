//
// Created by cowerling on 17-4-25.
//

#ifndef CREST_CRESTMODEL_H
#define CREST_CRESTMODEL_H

#include <string>
#include <ostream>

#include "../Core/Core.h"
#include "../Base/RasterCollection.h"

namespace CREST {
    class CRESTModel {
    public:
        CRESTModel();
        virtual ~CRESTModel();

        bool Build(const std::string &dem, const std::string &ddm, const std::string &fam, ProgressReport progress_report = nullptr);
        bool Simulate(const std::vector<std::string> &precipitations, const std::vector<std::string> &potential_evaporations, float time_interval, bool reset = false);
        bool Result(const std::string &file, StatusResultType type);

    protected:
        Raster *m_dem, *m_ddm, *m_fam;
        Core *m_core;
        ProgressReport m_progress_report;
    };
}

#endif //CREST_CRESTMODEL_H
