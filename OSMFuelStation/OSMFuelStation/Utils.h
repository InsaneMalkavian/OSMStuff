#pragma once
#include <string>
class Utils
{
public:
    Utils(void);
    ~Utils(void);
    static const double INVALID_LATLON;
    static const double LOCAL_PI;

    static double DegreesToRadians(const double degrees);
    static double Distance(const double lat1, const double lng1, const double lat2, const double lng2);
    static std::string IntToStr(const int ref);
    static std::string DoubleToStr(const double ref);
};
