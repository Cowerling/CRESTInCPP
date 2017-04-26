#include <iostream>
#include <ctime>
#include <sstream>

#include "Model/CRESTModel.h"

void Print(const std::string &message, int progess)
{
    std::cout << message << ' ' << progess << std::endl;
}



int main() {
    std::time_t now = time(0);
    std::tm *ltm = localtime(&now);
    ltm->tm_year = 103;
    ltm->tm_mon = 0;
    ltm->tm_mday = 1;
    ltm->tm_hour = 0;
    ltm->tm_min = 0;
    ltm->tm_sec = 0;

    std::time_t current_time = mktime(ltm);

    std::vector<std::string> precipitations, potential_evaporations;

    std::string dir = "/home/cowerling/Data/Nzoia/";

    for (int i = 0; i < 73; i++)
    {
        std::tm *ltm2 = localtime(&current_time);

        int year = 1900 + ltm2->tm_year, month = 1 + ltm2->tm_mon, day = ltm2->tm_mday, hour = ltm2->tm_hour;

        std::stringstream string_stream;
        string_stream << year << (month < 10 ? "0" : "") << month << (day < 10 ? "0" : "") << day << (hour < 10 ? "0" : "") << hour;
        std::string date_string;
        string_stream >> date_string;

        precipitations.push_back(dir + "precipitation/Rain." + date_string + ".tif");
        potential_evaporations.push_back(dir + "potential_evaporation/Pet." + date_string + ".tif");

        current_time += 3600 * 3;
    }

    std::string dem = "/home/cowerling/Data/Nzoia/DEM.tif", ddm = "/home/cowerling/Data/Nzoia/DDM.tif", fam = "/home/cowerling/Data/Nzoia/FAM.tif";

    CREST::CRESTModel model;
    model.Build(dem, ddm, fam, Print);
    model.Simulate(precipitations, potential_evaporations, 3);
    model.Result("/home/cowerling/Data/Nzoia/result/runoff.tif", RUNOFF);

    return 0;
}