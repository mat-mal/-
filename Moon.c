#include "Moon.h"

//calculations based on formulas from https://aa.quae.nl/en/reken/hemelpositie.html#4

void GetMoon(struct tm* pDate, double JD, double lat, double lon)
{
	double timeoffset = UTCOffset(pDate);

	MoonRaDec MoonRD;
	MoonRaDec* pMoonRD = &MoonRD;
	GetMoonRaDec(pMoonRD, JD);
	double moonculm = GetMoonCulminationTime(pMoonRD->ra, JD, timeoffset, lon);
	MoonRiseSet MoonRS;
	MoonRiseSet* pMoonRS = &MoonRS;
	GetMoonRiseSetTime(pMoonRS, pMoonRD->dec, lat, moonculm);
	double ilumi = GetIlumination(JD);

	PrintMoon(pMoonRS->t_rise, moonculm, pMoonRS->t_set, ilumi);
}

void GetMoonRaDec(MoonRaDec* MoonRD, double JD)
{
	double epsilon = 23.4397;
	double L = 218.316 + 13.176396*(JD - JD2000 - 0.5);
	double M = 134.963 + 13.064993*(JD - JD2000 - 0.5);
	double F = 93.272 + 13.229350*(JD - JD2000 - 0.5);
	double lambda = L + 6.289*sin(DegToRad(M));
	double betha = 5.128*sin(DegToRad(F));

	MoonRD->ra = atan2(sin(DegToRad(lambda))*cos(DegToRad(epsilon))-tan(DegToRad(betha))*sin(DegToRad(epsilon)),cos(DegToRad(lambda)));
	MoonRD->dec = asin(sin(DegToRad(betha))*cos(DegToRad(epsilon))+cos(DegToRad(betha))*sin(DegToRad(epsilon))*sin(DegToRad(lambda)));
}

double GetEarthMeanAnomaly(double JD)
{
	double M0 = 357.529;
	double n = 0.985608; 
	double M = Mod(M0 + n*(JD-JD2000),360);

	return M;
}

double GetMoonCulminationTime(double ra, double JD, int timeoffset, double lon)
{
	double M_earth = GetEarthMeanAnomaly(JD);
	double II_earth = 102.937;
	double maxh_time = Mod((RadToDeg(ra) - lon - M_earth - II_earth)/15.0 + timeoffset,24);
    
    return maxh_time;
}

void GetMoonRiseSetTime(MoonRiseSet* pMoonRS, double dec, double lat, double maxh_time)
{
    double horizon = RadToDeg(acos((sin(0.0)- sin(DegToRad(lat))*sin(dec))/(cos(DegToRad(lat))*cos(dec))));
    pMoonRS->t_rise = maxh_time - horizon/15;
    pMoonRS->t_set = maxh_time + horizon/15;
}

double GetIlumination(double JD)
{
	double moondistance = 384399000;
	double sundistance = 1.495978707E11;

	double L = 218.316 + 13.176396*(JD - JD2000 - 0.5);
	double M = 134.963 + 13.064993*(JD - JD2000 - 0.5);
	double F = 93.272 + 13.229350*(JD - JD2000 - 0.5);

	//calculating position of the Moon on the sky
	double lambda = L + 6.289*sin(DegToRad(M));
	double betha = 5.128*sin(DegToRad(F));

	double M_earth = GetEarthMeanAnomaly(JD);
	double II_earth = 102.937;
	double C_earth = 1.9148*sin(DegToRad(M_earth)) + 0.0200*sin(DegToRad(2*M_earth)) + 0.0003*sin(DegToRad(3*M_earth));
	double lambda_sun = M_earth + II_earth + C_earth + 180;
	double phi = RadToDeg(acos(cos(DegToRad(betha))*cos(DegToRad(lambda - lambda_sun))));										//calculating angle moon-earth-sun
	double sunmoondistance = sqrt(pow(moondistance,2) + pow(sundistance,2) - 2*moondistance*sundistance*cos(DegToRad(phi)));	//calculating distance between sun and moon
	double theta = RadToDeg(asin(sundistance*(sin(DegToRad(phi))/sunmoondistance)))-180;										//calculating angle earth-moon-sun
	double ilumi = (1+cos(DegToRad(theta)))/2.0*100.0;																			//calculating percentage of moon ilumination taking into account that moon is sphere (not disk)

	return ilumi;
}

void PrintMoon(double moonrise, double moonculm, double moonset, double ilumi)
{
	printf("\n");
	printf("MOON\n\n");
	printf("Rise time:   %2d:%02.f\n", (int)moonrise, GetMinutes(moonrise));
	printf("Lunar noon:  %2d:%02.f\n", (int)moonculm, GetMinutes(moonculm));
	printf("Set time:    %2d:%02.f\n", (int)moonset, GetMinutes(moonset));
	printf("Ilumination (%%): %.1f%%\n",ilumi);
}