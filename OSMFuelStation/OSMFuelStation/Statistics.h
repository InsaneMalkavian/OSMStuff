#ifndef STATS_H
#define STATS_H

#include <windows.h>
#define FRAME_BUFFER 256

class Statistics {
public:
	Statistics();
	~Statistics();
	void Start();								// starts or continues stopwatch
	double Stop();								// returns stopwatch and stops st
	void Reset();								// sets all stopwatch value to 0 
    void Restart();								// does reset and starts new timer
	double GetStWatch();						// just returns stopwatch
	double					TotalTime;			//total time
	double					TotalActiveTime;	//active time
private:
	LARGE_INTEGER			timerFrequency;		// QPC freq
	LARGE_INTEGER			timerStartFrame;	// QPC
	LARGE_INTEGER			timerEndFrame;
	LARGE_INTEGER			timerBegin;
	LARGE_INTEGER			timerCurrent;
	LARGE_INTEGER			stWatchStart;		// time when stopwatch started
	LARGE_INTEGER			stWatchNow;			// accumulative stopwatch value
};

#endif // STATS_H