#include "TimeSystem.h"

namespace Engine {

	namespace TimeSystem {

		_LARGE_INTEGER GameStartTime;
		_LARGE_INTEGER LastRecordedTime;
		_LARGE_INTEGER Frequency;
		float frameTime = 0;

		void Init() {
			QueryPerformanceFrequency(&Frequency);
			QueryPerformanceCounter(&GameStartTime);
			LastRecordedTime = GameStartTime;
		}

		void Update() {
			_LARGE_INTEGER temp, ElapsedTime;
			temp = LastRecordedTime;
			
			QueryPerformanceCounter(&LastRecordedTime);
			ElapsedTime.QuadPart = LastRecordedTime.QuadPart - temp.QuadPart;
			frameTime = static_cast<float>(ElapsedTime.QuadPart) / Engine::TimeSystem::GetFrequency().QuadPart;

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