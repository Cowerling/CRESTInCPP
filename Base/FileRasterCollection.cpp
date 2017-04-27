//
// Created by cowerling on 17-4-21.
//

#include "FileRasterCollection.h"

using namespace CREST;

FileRasterCollection::FileRasterCollection() : m_raster(nullptr), m_index(-1)
{}

FileRasterCollection::FileRasterCollection(const std::vector<std::string> &files) : m_raster(nullptr), m_index(-1)
{
    m_files = files;
}

FileRasterCollection::~FileRasterCollection()
{
    delete m_raster;
}

Raster* FileRasterCollection::operator[](int index)
{
    if (index == m_index) return m_raster;

    GDALDataset *dataset = static_cast<GDALDataset*>(GDALOpen(m_files[index].c_str(), GA_ReadOnly));

    if (dataset == nullptr) return m_raster = nullptr;

    m_raster = Raster::CreateRaster(dataset);

    return m_raster;
}

int FileRasterCollection::Size() const
{
    return m_files.size();
}

FileRasterCollection& FileRasterCollection::operator<<(const std::string &file)
{
    m_files.push_back(file);
    return *this;
}