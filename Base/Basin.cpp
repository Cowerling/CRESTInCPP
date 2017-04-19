//
// Created by cowerling on 17-4-19.
//
#include <cstring>

#include "Basin.h"

using namespace CREST;

Basin::Basin(GDALDataset *dem) : m_cells(nullptr), m_dem(dem)
{
    if (m_dem == nullptr) return;

    m_cells = new Cell*[GetYSize()];
    for (int i = 0; i < GetYSize(); i++)
        m_cells[i] = new Cell[GetXSize()];

    const char *projectionRef = m_dem->GetProjectionRef();
    char *wkt = new char[std::strlen(projectionRef)];
    std::strcpy(wkt, projectionRef);
    m_spatial_reference.importFromWkt(&wkt);    //it seems can not delete wkt
    if (!m_spatial_reference.IsGeographic() && !m_spatial_reference.IsProjected())
        m_spatial_reference.SetWellKnownGeogCS(Basin::DEFAULT_EPSG);
}

Basin::~Basin()
{
    for (int i = 0; i < GetYSize(); ++i)
        delete[] m_cells[i];
    delete[] m_cells;

    GDALClose(m_dem);
    GDALClose(m_ddm);
    GDALClose(m_fam);
}

int Basin::GetXSize() const
{
    return m_dem->GetRasterXSize();
}

int Basin::GetYSize() const
{
    return m_dem->GetRasterYSize();
}

const Cell& Basin::operator()(int x, int y) const
{
    return (*this)(x, y);
}

Cell& Basin::operator()(int x, int y)
{
    return m_cells[GetYSize() - 1 - y][x];
}

void Basin::SetFlowDirection(GDALDataset *ddm, GDALDataset *fam)
{
    m_ddm = ddm;
    m_fam = fam;
}

void Basin::FindCellFlowRoute(int x, int y)
{

}

void Basin::FindCellFlowRoute(Cell &cell)
{

}