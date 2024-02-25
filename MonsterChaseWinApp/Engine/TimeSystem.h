#pragma once

#include <Windows.h>

namespace Engine 
{
	namespace TimeSystem {

		_LARGE_INTEGER GameStartTime;
		_LARGE_INTEGER LastRecordedTime;
		LARGE_INTEGER Frequency;

		inline void Init() {
			QueryPerformanceFrequency(&Frequency);
			QueryPerformanceCounter(&GameStartTime);
			LastRecordedTime = GameStartTime;
		}

		inline void Update() {
			QueryPerformanceCounter(&LastRecordedTime);
		}
	}

}
