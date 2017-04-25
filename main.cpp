#include <iostream>
#include <ctime>

#include "Model/CRESTModel.h"

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

    for (int i = 0; i < 73; i++)
    {
        std::tm *ltm2 = localtime(&current_time);

        int year = 1900 + ltm2->tm_year, month = 1 + ltm2->tm_mon, day = ltm2->tm_mday, hour = ltm2->tm_hour;

        std::cout << year << (month < 10 ? "0" : "") << month << (day < 10 ? "0" : "") << day << (hour < 10 ? "0" : "") << hour << std::endl;

        current_time += 3600 * 3;
    }

    CREST::CRESTModel model;
    model.Build("", "", "", &std::cout);

    return 0;
}