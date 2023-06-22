#ifndef _JD_H_
#define _JD_H_
#include <stdio.h>
#include <time.h>
#include <unistd.h>

extern const double JD1970;
extern const double JD2000;

struct tm* pDate;

struct tm* CurrentDate(void);
struct tm* SetDate(char* date);
double GetJD(struct tm* Data);
int IsLeapYear(struct tm* pDate);
int UTCOffset(struct tm* pDate);
void PrintJD(double JD);
void PrintDate(struct tm* pDate);
#endif