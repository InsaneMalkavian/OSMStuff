#pragma once
#include <string>
#include <vector>
#include "FuelStation.h"

using namespace std;

class FuelStationReader
{
public:
    FuelStationReader(void);
    ~FuelStationReader(void);
    static vector<FuelStation*> GetLukoilJSON(const string& path);
    static vector<FuelStation*> GetAllOSMXML(const string& path);
    static vector<FuelStation*> GetData(const string& func, const string& path);
};