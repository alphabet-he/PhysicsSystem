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
			CollisionSystem::Init();
		}


		bool getQuit()
		{
			return g_bQuit;
		}

		void setQuit(bool val)
		{
			g_bQuit = val;
		}


		void ReleaseMemory()
		{
			RenderSystem::ReleaseAll();
			CollisionSystem::ReleaseCollidableList();
			PhysicsSystem::ReleasePhysicsList();
			MovableSystem::ReleaseMovableList();

			GameObjectController::ReleaseAll();

			Engine::GameObjectFactory::ReleaseRegisterFuncMaps();
		}

		void Update() {
			_LARGE_INTEGER currTime;
			QueryPerformanceCounter(&currTime);

			// Check collision before update velocity and acceleration
			CollisionSystem::CheckAllCollision(currTime);

			// Update velocity and acceleration
			PhysicsSystem::UpdateAll(currTime);

			// Update position
			MovableSystem::UpdateAll(currTime);

			// Render sprites
			RenderSystem::RenderAll();

			// Tell GLib we're done rendering sprites
			GLib::Sprites::EndRendering();
			// IMPORTANT: Tell GLib we're done rendering
			GLib::EndRendering();

			// update time system
			TimeSystem::Update();
		}
	}

}