#include "Statistics.h"


Statistics::Statistics() {
	TotalTime=0.0;
	TotalActiveTime=0.0;
	stWatchNow.QuadPart = 0.0f;
	DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
	::QueryPerformanceCounter(&timerBegin);
	::SetThreadAffinityMask(::GetCurrentThread(), oldmask);	
}

Statistics::~Statistics(void) {
	;//
}

void Statistics::Start() {
	QueryPerformanceFrequency(&timerFrequency);
	DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
	::QueryPerformanceCounter(&stWatchStart);
	::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
	}

double Statistics::Stop() {
	QueryPerformanceFrequency(&timerFrequency);
	DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
	LARGE_INTEGER TPF;
	::QueryPerformanceCounter(&TPF);
	::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
	stWatchNow.QuadPart = (TPF.QuadPart-stWatchStart.QuadPart+stWatchNow.QuadPart);
	stWatchStart.QuadPart = TPF.QuadPart;
	return (double)(stWatchNow.QuadPart) / timerFrequency.QuadPart;
	}

double Statistics::GetStWatch() {
	QueryPerformanceFrequency(&timerFrequency);
	DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
	LARGE_INTEGER TPF;
	::QueryPerformanceCounter(&TPF);
	::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
	TPF.QuadPart = (TPF.QuadPart-stWatchStart.QuadPart+stWatchNow.QuadPart);
	return (double)(TPF.QuadPart) / timerFrequency.QuadPart;
	}

void Statistics::Reset() {
	stWatchNow.QuadPart =0;
	stWatchStart.QuadPart =0;
	}

void Statistics::Restart() {
    Reset();
    Start();
}