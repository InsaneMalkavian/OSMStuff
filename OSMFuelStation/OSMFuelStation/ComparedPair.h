#pragma once
#include "FuelStation.h"

class ComparedPair : public FuelStation
{
public:
    enum ValidationState {
        ODD, // we have station in OSM, but we do not have it in list
        NOT_PRESENT, // we dont have such station in OSM
        INVALID, // valid only ref of coord and brand/op
        LOOKS_SIMILAR, // missed fuelIds and brand/operator
        VALID, // all major data is qual
        SIZE // leave this one the last one
    };
public:
    ComparedPair(void);
    ComparedPair(const FuelStation& ref);
    ~ComparedPair(void);
    void SetOriginal(const FuelStation& ref);
    void SetApplicant(const FuelStation& ref);
    string GetBrand() const;
    string GetOperator() const;
    string GetName() const;
    string GetRef() const;
    string GetLocalRef() const;
    string GetCoords() const;
    string GetCoordsOSM() const;
    string GetOffset() const;
    string GetDiesel() const;
    string Get80() const;
    string Get92() const;
    string Get95() const;
    string Get98() const;
    string GetPetroGas() const;
    string GetStatus() const;
    string GetColorStatus() const;
    static string GetColorStatus(const ValidationState mState);
    string GetLinkCoords(const string& mask) const;
public:    
    string mNameOSM;
    string mOperatorOSM;
    string mBrandOSM;
    double mLatOSM;
    double mLonOSM;
    double mOffset;
    int mRefOSM;
    int mLocalRefOSM;
    set<FuelTypes> mFuelTypesOSM;
    ValidationState mState;
private:
    bool ContainsFuel(const set<FuelTypes>& set, FuelTypes type) const;
    string GetFuelType(const FuelTypes type) const;
};
