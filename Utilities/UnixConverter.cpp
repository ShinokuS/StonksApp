#include "UnixConverter.h"
#include <time.h>
#include <stdlib.h>

TimeFormat* UnixConverter::timeConverter(time_t time)
{
	struct tm* tm = localtime(&time);
	char charHour[4];
	char charMinute[4];
	char charSecond[4];
	strftime(charHour, sizeof(charHour), "%H", tm);
	strftime(charMinute, sizeof(charMinute), "%M", tm);
	strftime(charSecond, sizeof(charSecond), "%S", tm);
	TimeFormat* convertedTime = new TimeFormat();
	convertedTime->hour = atoi(charHour);
	convertedTime->minute = atoi(charMinute);
	convertedTime->second = atoi(charSecond);
	return convertedTime;
}