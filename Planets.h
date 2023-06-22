#ifndef _PLANETS_H_
#define _PLANETS_H_
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "JD.h"
#include "Sun.h"
#include "mathfunc.h"

enum 
{
    planets = 8
};

double M[planets];
double nu[planets];
double r[planets];
double ha[planets];
double maxheight[planets];
double maxh_time[planets];
double horizon[planets];

struct tm* pDate;

typedef struct OrbitElem
{
    double a[planets];
    double e[planets];
    double in[planets];
    double om[planets];
    double OM[planets];
    double M0[planets];
    double n[planets];
    double a1e2[planets];
    double II[planets];
} Elements;

typedef struct HelioCoordinates
{
    double helio_x[planets];
    double helio_y[planets];
    double helio_z[planets];
} HelioCoords;

typedef struct GeoCoordinates
{
    double geo_x[planets];
    double geo_y[planets];
    double geo_z[planets];
} GeoCoords;

typedef struct EclipLoLa
{
    double DELTA[planets];
    double lambda[planets];
    double betha[planets];
} ELoLa;

typedef struct RightAscensionDeclination
{
    double ra[planets];
    double dec[planets];
} RaDec;

typedef struct PlanetRiseSet
{
    double t_rise[planets];
    double t_set[planets];
}PlanetRiseSet;

void GetPlanets(struct tm *pDate, double JD, double lat, double lon, bool onlyvisible);
void GetOrbitalElements(Elements* pElem);
void GetMeanAnomaly(double JD, Elements* pElements);
void GetTrueAnomaly(void);
void GetDistance(double* a1e2, double* e);
void GetHelioCoords(HelioCoords* pHCoords, double* OM, double* om, double* in);
void GetGeoCoords(GeoCoords* pGCoords, double* x, double* y, double* z);
void GetEclipLoLa(ELoLa* pELoLa, double* geo_x, double* geo_y, double* geo_z);
void GetRaDec(RaDec* pRaDec, double* lambda, double* beta);
double SidTime(double lon, double JD);
void GetHourAngle(double theta, double* ra);
void GetCulminationHeight(double* dec, double lat);
void GetCulminationTime(double* ra, double* II, int timeoffset, double lon);
void GetPlanetRiseSetTime(PlanetRiseSet* pPlanetRS, double* dec, double lat);
void PrintPlanets(RaDec* pRaDec, double* maxh_time, PlanetRiseSet* pPlanetRS, SunRiseSetCulm* pSunRSC, bool onlyvisible);


#endif