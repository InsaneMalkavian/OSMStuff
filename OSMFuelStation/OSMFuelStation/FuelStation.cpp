#include "FuelStation.h"
#include "Utils.h"

FuelStation::FuelStation(void) : mLat(Utils::INVALID_LATLON), mLon(Utils::INVALID_LATLON), mRef(-1), mLocalRef(-1), mID(0)
{
}

FuelStation::~FuelStation(void)
{
}

bool FuelStation::hasCoords() const{
    if (mLat == Utils::INVALID_LATLON || mLon == Utils::INVALID_LATLON)
        return false;
    return true;
}

double FuelStation::getLat() const {
    return mLat;
}

double FuelStation::getLon() const {
    return mLon;
}

void FuelStation::setLat(const double lat) {
    mLat = lat;
}

void FuelStation::setLon(const double lon) {
    mLon = lon;
}