#pragma once

#include <Windows.h>

namespace Engine 
{
	namespace TimeSystem {


		void Init();

		void Update();

		_LARGE_INTEGER GetGameStartTime();

		_LARGE_INTEGER GetLastRecordedTime();

		_LARGE_INTEGER GetFrequency();
	}

}
