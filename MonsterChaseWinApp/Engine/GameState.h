#pragma once
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "MovableSystem.h"
#include "GameObjectControllerSystem.h"
#include "TimeSystem.h"

namespace Engine 
{
	namespace GameState 
	{
		void Init();

		bool getQuit();

		void setQuit(bool val);

		void ReleaseMemory();
	}

}
