//
// Created by cowerling on 17-4-21.
//

#ifndef CREST_FILERASTERCOLLECTION_H
#define CREST_FILERASTERCOLLECTION_H

#include <string>
#include <vector>

#include "RasterCollection.h"

namespace CREST {
    class FileRasterCollection : RasterCollection {
    public:
        FileRasterCollection();
        FileRasterCollection(const std::vector<std::string> &files);

        virtual Raster* operator[](int index);
        void operator<<(const std::string &file);

        virtual ~FileRasterCollection();

    protected:
        Raster *m_raster;
        std::vector<std::string> m_files;
    };
}

#endif //CREST_FILERASTERCOLLECTION_H
