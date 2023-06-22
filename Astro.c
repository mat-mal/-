#include "Astro.h"

int main(int argc, char *argv[])
{
	double JD;
	char opt;
	char* setdate = NULL;
	struct tm* pDate;
	bool onlyvisible = true;

	while((opt = getopt(argc, argv, "at:lh")) != -1)
	{
		switch (opt)
		{
			case 'a':
				onlyvisible = false;
				break;
			case 't':
				setdate = optarg; 
				break;
			case 'l':
				remove("localization.data");
				break;
			case 'h':
			default:
				PrintManPage();
				return 0;
		}
	}
	Location Loc;
	Location* pLoc = &Loc;
	GetLocation(pLoc);
	if(setdate == NULL)
	{	
		pDate = CurrentDate();
		JD = GetJD(pDate);
	}
	else
	{
		pDate = SetDate(setdate);
		JD = GetJD(pDate);
	}
	PrintAll(JD, pDate, pLoc, onlyvisible);

	return 0;
}

void GetLocation(Location* pLoc)
{
	FILE* file = NULL;
	while(file == NULL)
	{
		int lat_d, lat_m, lon_d, lon_m;
		char lat, lon;
		file = fopen("./localization.data", "r");
		if(file == NULL)
		{
			fclose(file);
			printf("\nEnter the coordinates of your current location (Example: 51 28 N 0 00 E)\n");
			scanf(" %d %d %c %d %d %c", &lat_d, &lat_m, &lat, &lon_d, &lon_m, &lon);
			printf("\n");
			lat = toupper(lat);
			lon = toupper(lon);
			if(lat == 'S')
			{
				lat_d = lat_d * (-1);
			}
			if(lon == 'W')
			{
				lon_d = lon_d * (-1);
			}
			file = fopen("./localization.data", "w+");
			fprintf(file, "%d %d %c %d %d %c", lat_d, lat_m, lat, lon_d, lon_m, lon);
			rewind(file);
		}
		
		fscanf(file, " %d %d %c %d %d %c", &pLoc->lat_d, &pLoc->lat_m, &pLoc->lat, &pLoc->lon_d, &pLoc->lon_m, &pLoc->lon);
		if((pLoc->lat != 'S' && pLoc->lat != 'N') || (pLoc->lon != 'W' && pLoc->lon != 'E') || pLoc->lat_d < 0 || pLoc->lat_d > 90 || pLoc->lon_d < 0 || pLoc->lon_d > 180 || pLoc->lat_m < 0 || pLoc->lat_m > 60 || pLoc->lon_m < 0 || pLoc->lon_m > 60)
		{
			printf("ERROR! WRONG COORDINATES VALUE!\n");
			printf("Coordinates must be in format XX MM N/S YY MM E/W\n");
			printf("Where XX value must be between (0..90), YY value must be between (0..180), MM value must be between (0..60)\n");
			fclose(file);
			file = NULL;
			remove("localization.data");
			continue;
		}
		fclose(file);
	}
}

void PrintLocation(Location* pLoc)
{
	char deg = 248;
	printf("Your Location: %d%c%02d\' %c %d%c%02d\' %c\n", abs(pLoc->lat_d), deg, pLoc->lat_m, pLoc->lat, abs(pLoc->lon_d), deg, pLoc->lon_m, pLoc->lon);

}

void PrintAll(double JD, struct tm* pDate, Location* pLoc, bool onlyvisible)
{
	double lat = pLoc->lat_d+pLoc->lat_m/60.0;
	double lon = pLoc->lon_d+pLoc->lon_m/60.0;
	printf("\n");
	PrintDate(pDate);
	PrintLocation(pLoc);
	//PrintJD(JD);
	GetMoon(pDate, JD, lat, lon);
	GetSun(JD, pDate, lat, lon);
	GetPlanets(pDate, JD, lat, lon, onlyvisible);
}

void PrintManPage(void)
{
	printf("Usage: Astro.exe [options]\n");
	printf("Options:\n");
	printf("  -h                 Display this information\n");
	printf("  -t <date>          Define custom date (DD.MM.YYYY)\n");
	printf("  -a                 List all planets\n");
	printf("  -l                 Set new location\n");
}
