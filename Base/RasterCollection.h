//
// Created by cowerling on 17-4-21.
//

#ifndef CREST_RASTERCOLLECTION_H
#define CREST_RASTERCOLLECTION_H

#include "Raster.h"

namespace CREST {
    class RasterCollection {
    public:
        virtual Raster* operator[](int index) = 0;
        virtual ~RasterCollection();
    };
}

#endif //CREST_RASTERCOLLECTION_H
