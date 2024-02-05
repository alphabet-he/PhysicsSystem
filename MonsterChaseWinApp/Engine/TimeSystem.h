#pragma once

#include <Windows.h>

class TimeSystem
{
public:
	_LARGE_INTEGER GameStartTime;
	_LARGE_INTEGER LastRecordedTime;
	LARGE_INTEGER Frequency;

	TimeSystem() {
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&GameStartTime);
		LastRecordedTime = GameStartTime;
	}

	inline void Update() {
		QueryPerformanceCounter(&LastRecordedTime);
	}
};
