#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include "JD.h"
#include "Moon.h"
#include "Sun.h"
#include "Planets.h"

typedef struct Location
{
    int lat_d;
    int lat_m;
    char lat;
    int lon_d;
    int lon_m;
    char lon;
} Location;

void GetLocation(Location* pLoc);
void PrintLocation(Location* pLoc);
void PrintAll(double JD, struct tm* pDate, Location* coord, bool onlyvisible);
void PrintManPage(void);