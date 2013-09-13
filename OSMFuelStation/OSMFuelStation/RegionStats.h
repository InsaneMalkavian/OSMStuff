#pragma once
#include <string>

using namespace std;
struct RegionStats
{
    RegionStats(void);
    ~RegionStats(void);
    string name;
    string link;
    int states[5];
};

