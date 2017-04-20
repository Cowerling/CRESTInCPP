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
        inline double GetXResolution() const;
        inline double GetYResolution() const;
        const Cell& operator()(int x, int y) const;
        Cell& operator()(int x, int y);
        void SetFlowDirection(Raster *ddm, Raster *fam);
        void FindCellFlowRoute(int x, int y);

        void SetDEMCorrection(double dem_correction);

    private:
        void FindCellFlowRoute(Cell &cell);
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
    };

    inline double Basin::GetXResolution() const
    {
        return m_x_resolution;
    }

    inline double Basin::GetYResolution() const
    {
        return m_y_resolution;
    }
}

#endif //CREST_BASIN_H
