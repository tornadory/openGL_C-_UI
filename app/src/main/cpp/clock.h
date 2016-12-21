#pragma once

#include <sys/time.h>

inline double getTime()
{
	timespec t;

	clock_gettime(CLOCK_MONOTONIC_RAW, &t);

	return t.tv_sec + t.tv_nsec / 1000000000.;
}
