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
    bool hasFuelTypes() const;
    set<FuelTypes>& getFuelTypes() const;
    bool hasCoords() const;
    double getLat() const;
    double getLon() const;
    void setLat(const double lat);
    void setLon(const double lon);
    string mBrand;
    string mOperator;
    string mName;
    string mAddress;
protected:
    double mLat;
    double mLon;
public:
    int mRef;
    int mLocalRef;
    int mID;
    set<FuelTypes> mFuelTypes;
};