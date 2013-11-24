#include "ComparedPair.h"
#include <sstream>
#include "Utils.h"

ComparedPair::ComparedPair(void) : mLatOSM(Utils::INVALID_LATLON), mLonOSM(Utils::INVALID_LATLON), mOffset(-1.), mRefOSM(), mLocalRefOSM(-1), mState(NOT_PRESENT), mIDOSM(-1)
{
}

ComparedPair::~ComparedPair(void)
{
}

ComparedPair::ComparedPair(const FuelStation& ref) : mLatOSM(Utils::INVALID_LATLON), mLonOSM(Utils::INVALID_LATLON), mOffset(-1.), mRefOSM(), mLocalRefOSM(-1), mState(NOT_PRESENT), mIDOSM(-1) {
    mOriginal = ref;
}

void ComparedPair::SetOriginal(const FuelStation& ref) {
    mOriginal = ref;
}

void ComparedPair::SetApplicant(const FuelStation& ref) {
    mBrandOSM = ref.mBrand;
    mOperatorOSM = ref.mOperator;
    mNameOSM = ref.mName;
    mLatOSM = ref.getLat();
    mLonOSM = ref.getLon();
    mLocalRefOSM = ref.mLocalRef;
    mRefOSM = ref.mRef;
    mFuelTypesOSM = ref.mFuelTypes;
    mIDOSM = ref.mID;
}

string ComparedPair::GetBrand() const {
    return mOriginal.mBrand + " / " + mBrandOSM;
}

string ComparedPair::GetOperator() const {
    return mOriginal.mOperator + " / " + mOperatorOSM;
}

string ComparedPair::GetName() const {
    return mOriginal.mName + " / " + mNameOSM;
}

string ComparedPair::GetAddress() const {
    return mOriginal.mAddress;
}

string ComparedPair::GetRef() const {
    std::stringstream oss;
    oss << mOriginal.mRef << " / " << mRefOSM;
    return oss.str();
}

string ComparedPair::GetLocalRef() const {
    std::stringstream oss;
    oss << mOriginal.mLocalRef << " / " << mLocalRefOSM;
    return oss.str();
}

string ComparedPair::GetCoords() const {
    if (!mOriginal.hasCoords()) return string();
    std::stringstream oss;
    oss << mOriginal.getLat() <<", " << mOriginal.getLon();
    return oss.str();
}

string ComparedPair::GetCoordsOSM() const {
    if (mLatOSM==Utils::INVALID_LATLON || mLonOSM == Utils::INVALID_LATLON) return string();
    std::stringstream oss;
    oss << mLatOSM <<", " << mLonOSM;
    return oss.str();
}

string ComparedPair::GetOffset() const {
    if (mOffset<0) return string();
    std::stringstream oss;
    oss << mOffset;
    return oss.str();
}

string ComparedPair::GetDiesel() const {
    return GetFuelType(DIESEL);
}

string ComparedPair::Get80() const {
    return GetFuelType(AI80);
}

string ComparedPair::Get92() const {
    return GetFuelType(AI92);
}

string ComparedPair::Get95() const {
    return GetFuelType(AI95);
}

string ComparedPair::Get98() const {
    return GetFuelType(AI98);
}

string ComparedPair::GetPetroGas() const {
    return GetFuelType(PETGAS);
}

string ComparedPair::GetFuelType(const FuelTypes type) const {
    string ret;
    ContainsFuel(mOriginal.mFuelTypes, type) ? ret+="+" : ret+="-";
    ret+="/";
    ContainsFuel(mFuelTypesOSM, type) ? ret+="+" : ret+="-";
    return ret;
}

bool ComparedPair::ContainsFuel(const set<FuelTypes>& set, FuelTypes type) const {
    std::set<FuelTypes>::iterator it;
    it = set.find(type);
    if (it!=set.end())
        return true;
    return false;
}

string ComparedPair::GetStatus() const {
    string ret;
    switch (mState) {
    case VALID:
        ret = "Perfect match";
        break;
    case LOOKS_SIMILAR:
        ret = "Looks good";
        break;
    case INVALID:
        ret = "Invalid";
        break;
    case NOT_PRESENT:
        ret = "Not present";
        break;
    case ODD:
        ret = "Odd";
        break;
    }
    return ret;
}

string ComparedPair::GetColorStatus() const {
    return GetColorStatus(mState);
}

string ComparedPair::GetColorStatus(const ValidationState mState) {
    string ret;
    switch (mState) {
    case VALID:
        ret = "lime";
        break;
    case LOOKS_SIMILAR:
        ret = "yellow";
        break;
    case INVALID:
        ret = "coral";
        break;
    case NOT_PRESENT:
        ret = "lightgray";
        break;
    case ODD:
        ret = "white";
        break;
    }
    return ret;
}

string ComparedPair::GetLinkCoords(const string& mask) const {
    string ret = "http://openstreetmap.ru/#layer=M&zoom=17&lat=";
    ret+=Utils::DoubleToStr(mOriginal.getLat())+"&lon="+Utils::DoubleToStr(mOriginal.getLon());
    return ret;
}

string ComparedPair::GetOSMNodeURL(const string& mask) const {
    if (mIDOSM==-1) return string();
    string ret = "http://www.openstreetmap.org/browse/node/";
    ret+=Utils::IntToStr(mIDOSM);
    return ret;
}