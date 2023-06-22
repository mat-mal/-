#include "Sun.h"

void GetSun(double JD, struct tm* pDate, double lat, double lon)
{
	SunRiseSetCulm SunRSC;
	SunRiseSetCulm* pSunRSC = &SunRSC;
	GetSunRiseSetCulmTime(pSunRSC, JD, pDate, lat, lon);
	
	PrintSun(pSunRSC->t_rise, pSunRSC->t_set, pSunRSC->t_culm);
}

//formula based on "General Solar Position Calculations" by NOAA Global Monitoring Division
void GetSunRiseSetCulmTime(SunRiseSetCulm* pSunRSC, double JD, struct tm* pDate, double lat, double lon)
{
	int yday = pDate->tm_yday+1;
	int leap = IsLeapYear(pDate);
	int timeoffset = UTCOffset(pDate);
	double pi = M_PI;

	double gamma = (2.0*pi/(365.0+leap))*(yday-1+((pDate->tm_hour-12)/24)); 
	double eqtime = 229.18*(0.000075 + 0.001868*cos(gamma) - 0.032077*sin(gamma) - 0.014615*cos(2*gamma) - 0.040849*sin(2*gamma));
	double decl = 0.006918 - 0.399912*cos(gamma) + 0.070257*sin(gamma) - 0.006758*cos(2*gamma) + 0.000907*sin(2*gamma) - 0.002697*cos(3*gamma) + 0.00148*sin(3*gamma);
	double ha = RadToDeg(acos((cos(DegToRad(90.833))/(cos(DegToRad(lat))*cos(decl)))-tan(DegToRad(lat))*tan(decl)));
	pSunRSC->t_rise = (720 - 4.0*(lon+ha) - eqtime)/60.0 + timeoffset;
	pSunRSC->t_set = (720 - 4.0*(lon-ha) - eqtime)/60.0 + timeoffset;
	pSunRSC->t_culm = (720 - 4*lon - eqtime)/60.0 + timeoffset;
}

void PrintSun(double risetime, double settime, double culmtime)
{
	printf("\n");
	printf("SUN\n\n");
	printf("Rise time:   %2d:%02.f\n", (int)risetime, GetMinutes(risetime));
	printf("Solar noon:  %2d:%02.f\n", (int)culmtime, GetMinutes(culmtime));
	printf("Set time:    %2d:%02.f\n", (int)settime, GetMinutes(settime));
}