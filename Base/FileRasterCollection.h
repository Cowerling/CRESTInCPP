//
// Created by cowerling on 17-4-21.
//

#ifndef CREST_FILERASTERCOLLECTION_H
#define CREST_FILERASTERCOLLECTION_H

#include <string>
#include <vector>

#include "RasterCollection.h"

namespace CREST {
    class FileRasterCollection : public RasterCollection {
    public:
        FileRasterCollection();
        FileRasterCollection(const std::vector<std::string> &files);
        virtual ~FileRasterCollection();

        virtual Raster* operator[](int index);
        virtual int Size() const;
        FileRasterCollection& operator<<(const std::string &file);

    protected:
        Raster *m_raster;
        int m_index;
        std::vector<std::string> m_files;
    };
}

#endif //CREST_FILERASTERCOLLECTION_H
