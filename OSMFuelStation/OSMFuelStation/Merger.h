#pragma once
#include <vector>
#include "ComparedPair.h"
#include "FuelStation.h"
#include "RegionStats.h"

class Merger
{
private:
    enum Match {
    LOCALREF = 1<<0,
    COORDS = 1<<1,
    BRAND = 1<<2,
    OPERATOR = 1<<3,
    FUEL = 1<<4,
    ST_VALID = LOCALREF | COORDS | BRAND | OPERATOR | FUEL,
    NAME = 1<<5
};
public:
    Merger(void);
    ~Merger(void);
    vector<ComparedPair*> CompareLists(const vector<FuelStation*>& original, const vector<FuelStation*>& applicants, RegionStats& stats) const;
    ComparedPair ComparePair(const FuelStation& original, const FuelStation& applicant) const;
    ComparedPair ComparePair(const FuelStation* original, const FuelStation* applicant) const;
private:
    static const double OFFSET_LIMIT;
};

