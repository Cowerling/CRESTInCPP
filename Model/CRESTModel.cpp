//
// Created by cowerling on 17-4-25.
//

#include "CRESTModel.h"
#include "../Base/FileRasterCollection.h"

using namespace CREST;

CRESTModel::CRESTModel() : m_core(nullptr), m_dem(nullptr), m_ddm(nullptr), m_fam(nullptr), m_progress_report(nullptr)
{
    GDALAllRegister();
}

CRESTModel::~CRESTModel()
{
    delete m_core;
    delete m_dem;
    delete m_ddm;
    delete m_fam;
}

bool CRESTModel::Build(const std::string &dem, const std::string &ddm, const std::string &fam, ProgressReport progress_report)
{
    try
    {
        m_progress_report = progress_report;

        GDALDataset *dem_dataset = static_cast<GDALDataset*>(GDALOpen(dem.c_str(), GA_ReadOnly)),
                *ddm_dataset = static_cast<GDALDataset*>(GDALOpen(ddm.c_str(), GA_ReadOnly)),
                *fam_dataset = static_cast<GDALDataset*>(GDALOpen(fam.c_str(), GA_ReadOnly));

        if (dem_dataset == nullptr || ddm_dataset == nullptr || fam_dataset == nullptr)
        {
            if (m_progress_report != nullptr) (*m_progress_report)("DEM or DDM or FAM is null", 0);

            return false;
        }

        m_dem = Raster::CreateRaster(dem_dataset, dem_dataset->GetRasterXSize(), Raster::INIT_Y_EXTENT);
        m_ddm = Raster::CreateRaster(ddm_dataset, ddm_dataset->GetRasterXSize(), Raster::INIT_Y_EXTENT);
        m_fam = Raster::CreateRaster(fam_dataset, fam_dataset->GetRasterXSize(), Raster::INIT_Y_EXTENT);

        return true;
    }
    catch (const std::runtime_error &exception)
    {
        if (m_progress_report != nullptr) (*m_progress_report)("build failed: " + std::string(exception.what()), 0);

        return false;
    }
    catch (...)
    {
        if (m_progress_report != nullptr) (*m_progress_report)("build failed", 0);

        return false;
    }
}

bool CRESTModel::Simulate(const std::vector<std::string> &precipitations,
                          const std::vector<std::string> &potential_evaporations, float time_interval, bool reset)
{
    try
    {
        if (reset)
        {
            delete m_core;
            m_core = nullptr;
        }

        if (m_core == nullptr) m_core = new Core(m_dem, m_ddm, m_fam, m_progress_report);

        FileRasterCollection precipitation_collection = FileRasterCollection(precipitations),
                potential_evaporation_collection = FileRasterCollection(potential_evaporations);

        m_core->Caculate(precipitation_collection, potential_evaporation_collection, time_interval);

        return true;
    }
    catch (const std::runtime_error &exception)
    {
        if (m_progress_report != nullptr) (*m_progress_report)("build failed: " + std::string(exception.what()), 0);

        return false;
    }
    catch (...)
    {
        if (m_progress_report != nullptr) (*m_progress_report)("simulate failed", 0);

        return false;
    }
}

bool CRESTModel::Result(const std::string &file, StatusResultType type)
{
    try
    {
        if (m_core == nullptr) return false;

        GDALDriver *driver = GetGDALDriverManager()->GetDriverByName("GTiff");

        if (driver == nullptr) return false;

        GDALDataset *dataset = driver->Create(file.c_str(), m_dem->GetXSize(), m_dem->GetYSize(), 1, GDT_Float32, NULL);
        m_core->GetResult(dataset, type);

        return true;
    }
    catch (const std::runtime_error &exception)
    {
        if (m_progress_report != nullptr) (*m_progress_report)("build failed: " + std::string(exception.what()), 0);

        return false;
    }
    catch (...)
    {
        if (m_progress_report != nullptr) (*m_progress_report)("get result failed", 0);

        return false;
    }
}
