#include "TimeSystem.h"

namespace Engine {

	namespace TimeSystem {

		_LARGE_INTEGER GameStartTime;
		_LARGE_INTEGER LastRecordedTime;
		_LARGE_INTEGER Frequency;

		void Init() {
			QueryPerformanceFrequency(&Frequency);
			QueryPerformanceCounter(&GameStartTime);
			LastRecordedTime = GameStartTime;
		}

		void Update() {
			QueryPerformanceCounter(&LastRecordedTime);
		}

		_LARGE_INTEGER GetGameStartTime()
		{
			return GameStartTime;
		}

		_LARGE_INTEGER GetLastRecordedTime()
		{
			return LastRecordedTime;
		}

		_LARGE_INTEGER GetFrequency()
		{
			return Frequency;
		}


	}

}