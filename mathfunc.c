#include "mathfunc.h"

const double pi = M_PI;

//because C modulo function (%) sucks
double Mod(double A, double B)
{
	double modulo = A - B*floor((A/B));
	return modulo;
}

double RadToDeg(double rad)
{
	double deg = rad*(180.0/pi);
	return deg;
}

double DegToRad(double deg)
{
	double rad = deg*(pi/180.0);
	return rad;
}

double DegToH(double deg)
{
    double H = deg/15.0;
    return H;
}

double RadToH(double rad)
{
    double H = rad*12.0/pi;
    return H;
}

double GetMinutes(double degh)
{
	double minutes = fabs((degh - (int)degh)*60.0);
	return minutes;
}