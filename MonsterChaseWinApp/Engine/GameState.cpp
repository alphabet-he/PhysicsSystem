#include "GameState.h"

namespace Engine
{
	namespace GameState
	{
		bool g_bQuit = false;

		void Init() {
			// Initialize systems
			GameObjectController::Init();
			TimeSystem::Init();
			RenderSystem::Init();
			PhysicsSystem::Init();
			MovableSystem::Init();
		}


		bool getQuit()
		{
			return g_bQuit;
		}

		void setQuit(bool val)
		{
			g_bQuit = val;
		}
	}

}