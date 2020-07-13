#pragma once

#include "TimeFormat.h"

class UnixConverter
{
public:
	TimeFormat* timeConverter(time_t time);
};