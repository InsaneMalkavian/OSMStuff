#pragma once
#include <string>
#include <vector>
#include <set>
using namespace std;

enum FuelTypes {DIESEL, AI80, AI92, AI95, AI98, PETGAS};

struct FuelStation
{
public:
    FuelStation(void);
    virtual ~FuelStation(void);

public:
    string mBrand;
    string mOperator;
    string mName;
    string mAddress;
    double mLat;
    double mLon;
    int mRef;
    int mLocalRef;
    set<FuelTypes> mFuelTypes;
};