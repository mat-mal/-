#include "JD.h"

const double JD1970 = 2440587.50000;			//Julian day at 00:00:00 UTC on 1 January 1970
const double JD2000 = 2451544.5;

struct tm* CurrentDate(void)
{
	time_t secondssince1970 = time(NULL);						//number of seconds that have elapsed since 00:00:00 UTC on 1 January 1970
	pDate = localtime(&secondssince1970);
	return pDate;
}

struct tm* SetDate(char* date)
{
	int day, month, year;
	static struct tm setdate = {0};
	sscanf(date," %d.%d.%d", &day, &month, &year);
	if(day > 31 || day < 1 || month > 12 || month < 1  || year < 1970 || year > 3000)
	{
		printf("ERROR! WRONG DATE VALUE!\n");
		printf("Date must be in format DD.MM.YYYY\n");
		printf("Where DD value must be between (1..31), MM value must be between (1..12) and YYYY value must be between (1970..3000)\n");
		exit(0);
	}
	setdate.tm_year = year - 1900;
	setdate.tm_mon = month - 1;
	setdate.tm_mday = day;
	setdate.tm_hour = 1;
	setdate.tm_min = 0;
	setdate.tm_sec = 0;
	setdate.tm_isdst = 0;
	pDate = &setdate;
	return pDate;
}

double GetJD(struct tm* pDate)
{
	time_t secondssince1970 = mktime(pDate);
	double JD = JD1970+secondssince1970/86400.0;

	return JD;
}

int IsLeapYear(struct tm* pDate)
{
	int year = pDate->tm_year+1900;
	int leapyear = 0;
	if(year % 400 == 0 || (year % 4 == 0 && !(year % 100 == 0)))
	{
		leapyear = 1;
	}
	//printf("leapyear: %d\n",leapyear);

	return leapyear;
}

int UTCOffset(struct tm* pDate)					
{
	
	time_t secondssince1970 = mktime(pDate);					//mktime must be first 
	struct tm* gmt = gmtime(&secondssince1970);
	int gmt_hour = gmt->tm_hour;
	pDate = localtime(&secondssince1970);
	int local_hour = pDate->tm_hour;
	int t_offset = local_hour-gmt_hour;
	
	return t_offset;
}

void PrintJD(double JD)
{
	printf("JD: %lf\n", JD);
}

void PrintDate(struct tm* pDate)
{
	printf("Date: %i.%02i.%04i\n",pDate->tm_mday, pDate->tm_mon+1, pDate->tm_year+1900);
}