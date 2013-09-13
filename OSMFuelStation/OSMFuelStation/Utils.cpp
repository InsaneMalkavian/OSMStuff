#include "Utils.h"
#include <math.h>
#include <sstream>

const double Utils::INVALID_LATLON = 999.;
const double Utils::LOCAL_PI = 3.1415926535897932385;

Utils::Utils(void)
{
}

Utils::~Utils(void)
{
}

double Utils::DegreesToRadians(const double degrees) {
    double radians = degrees * LOCAL_PI / 180;
    return radians;
}

double Utils::Distance(const double lat1, const double lng1, const double lat2, const double lng2) {
    double earthRadius = 6371;
    double dLat = DegreesToRadians(lat2-lat1);
    double dLng = DegreesToRadians(lng2-lng1);
    double a = sin(dLat/2) * sin(dLat/2) + 
        cos(DegreesToRadians(lat1)) * cos(DegreesToRadians(lat2)) * sin(dLng/2) * sin(dLng/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double dist = earthRadius * c;
    return dist;
}

std::string Utils::IntToStr(const int ref) {
    std::stringstream oss;
    oss<< ref;
    return oss.str();
}

std::string Utils::DoubleToStr(const double ref) {
    std::stringstream oss;
    oss<< ref;
    return oss.str();
}
