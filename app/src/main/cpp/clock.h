#pragma once

#include <sys/time.h>

inline double getTime()
{
	timespec t;

	clock_gettime(CLOCK_MONOTONIC_RAW, &t);

	return t.tv_sec + t.tv_nsec / 1000000000.;
}


struct TimerOur
{
	double getCurrentTime()
	{
		return getTime()-_pause_total_time;
	}

	 void setpauseTime(double i_time)
	 {
		 _pause_time=i_time;
	 }

	 void setResumeTime(double i_time)
	 {
		 if(_pause_time!=0.0f)
		 {
			 _pause_total_time=_pause_total_time+i_time-_pause_time;
			 _pause_time=0.0f;
		 }

	 }

//	void pause()
//	{
//		_pause_time = getTime();
//	}
//
//	void resume()
//	{
//		_pause_total_time += getTime() - _pause_time;
//	}

	double _pause_total_time=0.0f;
	double _pause_time= 0; //getTime();
};
