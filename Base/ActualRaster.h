//
// Created by cowerling on 17-4-19.
//

#ifndef CREST_ACTUALRASTER_H
#define CREST_ACTUALRASTER_H

#include "Raster.h"

namespace CREST {
    template<class T> class ActualRaster : public Raster {
    public:
        ActualRaster(GDALDataset *dataset);
        virtual ~ActualRaster();

        virtual double GetValue(int x, int y) const;

    protected:
        T *data;
    };

    template<class T> ActualRaster::ActualRaster(GDALDataset *dataset) : Raster(dataset)
    {}

    template<class T> virtual ActualRaster::~ActualRaster()
    {
        delete[] data;
    }

    template<class T> virtual double ActualRaster::GetValue(int x, int y) const
    {

    }
}

#endif //CREST_ACTUALRASTER_H
