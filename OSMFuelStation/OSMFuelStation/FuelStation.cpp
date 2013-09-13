#include "FuelStation.h"
#include "Utils.h"

FuelStation::FuelStation(void) : mLat(Utils::INVALID_LATLON), mLon(Utils::INVALID_LATLON), mRef(-1), mLocalRef(-1)
{
}


FuelStation::~FuelStation(void)
{
}
