//
// Created by cowerling on 17-4-19.
//

#ifndef CREST_BASIN_H
#define CREST_BASIN_H

#include <gdal_priv.h>
#include <ogrsf_frmts.h>

#include "Cell.h"

namespace CREST {
    class Basin {
    public:
        Basin(GDALDataset *dem);
        ~Basin();

        int GetXSize() const;
        int GetYSize() const;
        inline const OGRSpatialReference& GetSpatialReference() const;
        const Cell& operator()(int x, int y) const;
        Cell& operator()(int x, int y);
        void SetFlowDirection(GDALDataset *ddm, GDALDataset *fam);
        void FindCellFlowRoute(int x, int y);


    private:
        void FindCellFlowRoute(Cell &cell);
        void CaculateFlowTime(Cell &cell);

    protected:
        Cell **m_cells;
        GDALDataset *m_dem, *m_ddm, *m_fam;
        OGRSpatialReference m_spatial_reference;

    private:
        const static char *DEFAULT_EPSG = "EPSG:4326";
    };

    inline const OGRSpatialReference& Basin::GetSpatialReference() const
    {
        return m_spatial_reference;
    }
}

#endif //CREST_BASIN_H
