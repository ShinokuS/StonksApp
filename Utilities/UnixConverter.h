#pragma once

#include "TimeFormat.h"

#include <time.h>

class UnixConverter
{
public:
	static TimeFormat* timeConverter(time_t time = time(0));
};