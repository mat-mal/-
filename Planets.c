#include "Planets.h"

//all calculation is based on formulas from https://www.aa.quae.nl/en/reken/hemelpositie.html

void GetPlanets(struct tm *pDate, double JD, double lat, double lon, bool onlyvisible)
{
    Elements Elem;
    Elements* pElem = &Elem;
    GetOrbitalElements(pElem);
    GetMeanAnomaly(JD, pElem);
    GetTrueAnomaly();
    GetDistance(pElem->a1e2, pElem->e);
    HelioCoords HCoords;
    HelioCoords* pHCoords = &HCoords;
    GetHelioCoords(pHCoords, pElem->OM, pElem->om, pElem->in);
    GeoCoords GCoords;
    GeoCoords* pGCoords = &GCoords;
    GetGeoCoords(pGCoords, pHCoords->helio_x, pHCoords->helio_y, pHCoords->helio_z);
    ELoLa EcliLoLa;
    ELoLa* pElola = &EcliLoLa;
    GetEclipLoLa(pElola, pGCoords->geo_x, pGCoords->geo_y, pGCoords->geo_z);
    RaDec RD;
    RaDec* pRD = &RD;
    GetRaDec(pRD, pElola->lambda, pElola->betha);
    double theta = SidTime(lon, JD);
    GetHourAngle(theta, pRD->ra);
    GetCulminationHeight(pRD->dec, lat);
    double timeoffset = UTCOffset(pDate);
    GetCulminationTime(pRD->ra, pElem->II, timeoffset, lon);
    PlanetRiseSet PlanetRS;
    PlanetRiseSet* pPlanetRS = &PlanetRS;
    GetPlanetRiseSetTime(pPlanetRS, pRD->dec, lat);
    SunRiseSetCulm SunRSC; 
    SunRiseSetCulm* pSunRSC = &SunRSC;
    GetSunRiseSetCulmTime(pSunRSC, JD, pDate, lat, lon);

    PrintPlanets(pRD, maxh_time, pPlanetRS, pSunRSC, onlyvisible);
}

void GetOrbitalElements(Elements* pElem)
{
    FILE* file = NULL;
    file = fopen("./OrbitalElements.data", "r");

    for(int i = 0; i < planets; i++)
    {
        fscanf(file, " %lf %lf %lf %lf %lf %lf %lf %lf %lf", &pElem->a[i], &pElem->e[i], &pElem->in[i], &pElem->om[i], &pElem->OM[i], &pElem->M0[i], &pElem->n[i], &pElem->a1e2[i], &pElem->II[i]);
        //printf("%lf %lf %lf %lf %lf %lf %lf %lf %lf\n", pElements->a[i], pElements->e[i], pElements->in[i], pElements->om[i], pElements->OM[i], pElements->M0[i], pElements->n[i], pElements->a1e2[i], pElements->II[i]);
    }
    fclose(file);
}

void GetMeanAnomaly(double JD, Elements* pElements)
{
    for(int i = 0; i < planets; i++)
    {
        M[i] = Mod(pElements->M0[i] + pElements->n[i]*(JD-JD2000),360);
        //printf("M%d: %f\n",i+1, M[i]);
    }
}

//calculation based on formulas from https://aa.quae.nl/en/reken/zonpositie.html
void GetTrueAnomaly(void)
{
    double C[planets];

    C[0] = 23.4400*sin(DegToRad(M[0])) + 2.9818*sin(DegToRad(2*M[0])) + 0.5255*sin(DegToRad(3*M[0])) + 0.1058*sin(DegToRad(4*M[0])) + 0.0241*sin(DegToRad(5*M[0])) + 0.0055*sin(DegToRad(6*M[0]));
    C[1] = 0.7758*sin(DegToRad(M[1])) + 0.0033*sin(DegToRad(2*M[1]));
    C[2] = 1.9148*sin(DegToRad(M[2])) + 0.0200*sin(DegToRad(2*M[2])) + 0.0003*sin(DegToRad(3*M[2]));
    C[3] = 10.6912*sin(DegToRad(M[3])) + 0.6228*sin(DegToRad(2*M[3])) + 0.0503*sin(DegToRad(3*M[3])) + 0.0046*sin(DegToRad(4*M[3])) + 0.0005*sin(DegToRad(5*M[3]));
    C[4] = 5.5549*sin(DegToRad(M[4])) + 0.1683*sin(DegToRad(2*M[4])) + 0.0071*sin(DegToRad(3*M[4])) + 0.0003*sin(DegToRad(4*M[4])); 
    C[5] = 6.3585*sin(DegToRad(M[5])) + 0.2204*sin(DegToRad(2*M[5])) + 0.0106*sin(DegToRad(3*M[5])) + 0.0006*sin(DegToRad(4*M[5]));
    C[6] = 5.3042*sin(DegToRad(M[6])) + 0.1534*sin(DegToRad(2*M[6])) + 0.0062*sin(DegToRad(3*M[6])) + 0.0003*sin(DegToRad(4*M[6]));
    C[7] = 1.0302*sin(DegToRad(M[7])) + 0.0058*sin(DegToRad(2*M[7]));

    for(int i = 0; i < planets; i++)
    {
        nu[i] = M[i] + C[i];
       //printf("nu%d: %f\n",i+1, nu[i]);
    }
}

void GetDistance(double* a1e2, double* e)
{
        for(int i = 0; i < planets; i++)
    {
        r[i] = a1e2[i]/(1+e[i]*cos(DegToRad(nu[i])));
        //printf("r%d: %f\n",i+1, r[i]);
    }
}

void GetHelioCoords(HelioCoords* pHCoords, double* OM, double* om, double* in)
{
    for(int i = 0; i < planets; i++)
    {
        pHCoords->helio_x[i] = r[i]*(cos(DegToRad(OM[i]))*cos(DegToRad(om[i]+nu[i]))-sin(DegToRad(OM[i]))*cos(DegToRad(in[i]))*sin(DegToRad(om[i]+nu[i])));
        pHCoords->helio_y[i] = r[i]*(sin(DegToRad(OM[i]))*cos(DegToRad(om[i]+nu[i]))+cos(DegToRad(OM[i]))*cos(DegToRad(in[i]))*sin(DegToRad(om[i]+nu[i])));
        pHCoords->helio_z[i] = r[i]*sin(DegToRad(in[i]))*sin(DegToRad(om[i]+nu[i]));

        //printf("x%d: %f\n",i+1, pHCoords->helio_x[i]);
        //printf("y%d: %f\n",i+1, pHCoords->helio_y[i]);
        //printf("z%d: %f\n",i+1, pHCoords->helio_z[i]);
    }
}

void GetGeoCoords(GeoCoords* pGCoords, double* x, double* y, double* z)
{
    for(int i = 0; i < planets; i++)
    {
        pGCoords->geo_x[i] = x[i] - x[2];
        pGCoords->geo_y[i] = y[i] - y[2];
        pGCoords->geo_z[i] = z[i] - z[2];

        //printf("xE%d: %f\n",i+1, pGCoords->geo_x[i]);
        //printf("yE%d: %f\n",i+1, pGCoords->geo_y[i]);
        //printf("zE%d: %f\n",i+1, pGCoords->geo_z[i]);
    }
}

void GetEclipLoLa(ELoLa* pELoLa, double* geo_x, double* geo_y, double* geo_z)
{
    for(int i = 0; i < planets; i++)
    {
        pELoLa->DELTA[i] = sqrt(pow(geo_x[i],2)+pow(geo_y[i],2)+pow(geo_z[i],2));
        pELoLa->lambda[i] = atan2(geo_y[i],geo_x[i]);         //radians
        pELoLa->betha[i] = asin(geo_z[i]/pELoLa->DELTA[i]);         //radians

        //printf("DELTA%d: %f\n",i+1, pELoLa->DELTA[i]);
        //printf("lambda%d: %f\n",i+1, pELoLa->lambda[i]);
        //printf("beta%d: %f\n",i+1, pELoLa->beta[i]);
    }
}

void GetRaDec(RaDec* pRaDec, double* lambda, double* betha)
{
    double epsilon = 23.4397;
    for(int i = 0; i < planets; i++)
    {
        pRaDec->ra[i] = atan2(sin(lambda[i])*cos(DegToRad(epsilon))-tan(betha[i])*sin(DegToRad(epsilon)),cos(lambda[i]));
        pRaDec->dec[i] = asin(sin(betha[i])*cos(DegToRad(epsilon))+cos(betha[i])*sin(DegToRad(epsilon))*sin(lambda[i]));
    }
}

double SidTime(double lon, double JD)
{
    double dJD = JD - JD2000;
    double L0 = 99.967794687;
    double L1 = 360.98564736628603;
    long double L2 = 2.907879E-13;
    long double L3 = -5.302E-22;
    double theta = L0 + L1*dJD + L2*pow(dJD,2) + L3*pow(dJD,3) + lon;
        
    //printf("theta: %f\n", Mod(DegToH(theta),24));

    return theta;
}

void GetHourAngle(double theta, double* ra)
{
    for(int i = 0; i < planets; i++)
    {
        ha[i] = theta - RadToDeg(ra[i]);
        
        //printf("Hour angle%d: %f\n",i+1, Mod(DegToH(ha[i]),24));
    }
}

void GetCulminationHeight(double* dec, double lat)
{
    for(int i = 0; i < planets; i++)
    {
        maxheight[i] = RadToDeg(asin(sin(DegToRad(lat))*sin(dec[i])+cos(DegToRad(lat))*cos(dec[i])*cos(0)));

        //printf("MaxH%d: %f\n",i+1, maxheight[i]);
    }
}

void GetCulminationTime(double* ra, double* II, int timeoffset, double lon)
{
    for(int i = 0; i < planets; i++)
    {
        maxh_time[i] = Mod((RadToDeg(ra[i]) - lon - M[2] - II[2])/15.0 + timeoffset,24);

    }
}

void GetPlanetRiseSetTime(PlanetRiseSet* pPlanetRS, double* dec, double lat)
{
    for(int i = 0; i < planets; i++)
    {
        horizon[i] = RadToDeg(acos((sin(0.0)- sin(DegToRad(lat))*sin(dec[i]))/(cos(DegToRad(lat))*cos(dec[i]))));
        pPlanetRS->t_rise[i] = maxh_time[i] - horizon[i]/15;
        pPlanetRS->t_set[i] = maxh_time[i] + horizon[i]/15;

        //printf("Horizon%d: %f\n", i+1, horizon[i]);
    }
}

void PrintPlanets(RaDec* pRaDec, double* maxh_time, PlanetRiseSet* pPlanetRS, SunRiseSetCulm* pSunRSC, bool onlyvisible)
{   

    char* planetname[] = {"Mercury", " Venus ", "  Earth", "  Mars ", "Jupiter", " Saturn", " Uranus", "Neptune"};
    char* table[] = {"Planet         ", "Right Ascention", "Declination    ", "Rise time      ", "Culm. time     ", "Set time       "};
    int visible = 0;
    double planetsdata[5][8];
    for(int i = 0; i < planets; i++)
    {
        planetsdata[0][i] = Mod(RadToH(pRaDec->ra[i]), 24);
        planetsdata[1][i] = RadToDeg(pRaDec->dec[i]);
        planetsdata[2][i] = Mod(pPlanetRS->t_rise[i], 24);
        planetsdata[3][i] = Mod(maxh_time[i], 24);
        planetsdata[4][i] = Mod(pPlanetRS->t_set[i], 24);
    }
    printf("\n");
    printf("PLANETS\n\n");
    for(int j = 0; j < 6; j++)
    {
        printf("%s |", table[j]);
        for(int i = 0; i < planets; i++)
        {
            if(i == 2) continue;
            if(!(pPlanetRS->t_rise[i]+1 < pSunRSC->t_rise || pPlanetRS->t_set[i]+1 > pSunRSC->t_set) && onlyvisible) continue;
            if(i > 5 && onlyvisible) continue;
            if(j == 0)
            {
                visible++;
                printf(" %s |", planetname[i]);
            }
            else if(j == 1)
            {
                printf(" %3dh%02.fm |", (int)planetsdata[j-1][i], GetMinutes(planetsdata[j-1][i]));
            }                
            else if(j == 2)
            {
                char deg = 248;
                printf(" %3d%c%02.f\' |", (int)planetsdata[j-1][i], deg, GetMinutes(planetsdata[j-1][i]));
            }
            else
            {
                printf(" %3d:%02.f  |", (int)planetsdata[j-1][i], GetMinutes(planetsdata[j-1][i]));
            } 
        }
        if(j == 0)
        {
            printf("\n----------------|");
            for(int i = 0; i < visible; i++)
            {
                printf("---------|");
            }
        }
        printf("\n");
    }
}