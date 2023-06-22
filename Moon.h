#ifndef _MOON_H_
#define _MOON_H_
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>
#include "JD.h"
#include "mathfunc.h"

typedef struct MoonRightAscensionDeclination
{
    double ra;
    double dec;
} MoonRaDec;

typedef struct MoonRiseSet
{
    double t_rise;
    double t_set;
}MoonRiseSet;

void GetMoon(struct tm* pDate, double JD, double lat, double lon);
void GetMoonRaDec(MoonRaDec* MoonRD, double JD);
double GetEarthMeanAnomaly(double JD);
double GetMoonCulminationTime(double ra, double JD, int timeoffset, double lon);
void GetMoonRiseSetTime(MoonRiseSet* pMoontRS, double dec, double lat, double maxh_time);
double GetIlumination(double JD);
void PrintMoon(double moonrise, double moonculm, double moonset, double ilumi);
#endif