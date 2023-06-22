#ifndef _SUN_H_
#define _SUN_H_
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "JD.h"
#include "mathfunc.h"

typedef struct SunRiseSetCulmination
{
    double t_rise;
    double t_culm;
    double t_set;
}SunRiseSetCulm;

void GetSun(double JD, struct tm* pDate, double lat, double lon);
void GetSunRiseSetCulmTime(SunRiseSetCulm* pSunRSC, double JD, struct tm* pDate, double lat, double lon);
void PrintSun(double risetime, double settime, double culmtime);
#endif