#include "UnixConverter.h"
#include <time.h>
#include <stdlib.h>

TimeFormat* UnixConverter::timeConverter(time_t time)
{
	struct tm* tm = localtime(&time);
	char charHour[2];
	char charMinute[2];
	strftime(charHour, sizeof(charHour), "%H", tm);
	strftime(charMinute, sizeof(charMinute), "%M", tm);
	TimeFormat* convertedTime = new TimeFormat();
	convertedTime->hour = atoi(charHour);
	convertedTime->minute = atoi(charMinute);
	return convertedTime;
}