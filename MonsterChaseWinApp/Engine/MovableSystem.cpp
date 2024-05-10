#include "MovableSystem.h"
#include "TimeSystem.h"

namespace Engine {
	namespace MovableSystem {
		std::vector<std::weak_ptr<GameObject>> AllMovables;

		void Update(std::shared_ptr<GameObject> go, _LARGE_INTEGER currTime) {
			// update position
			// find delta-t
			_LARGE_INTEGER ElapsedTime;
			ElapsedTime.QuadPart = currTime.QuadPart - Engine::TimeSystem::GetLastRecordedTime().QuadPart;
			float ElapsedSeconds = static_cast<float>(ElapsedTime.QuadPart) / Engine::TimeSystem::GetFrequency().QuadPart;

			MovableComponent* movable = static_cast<MovableComponent*>(go->GetComponent("MovableComponent"));
			if (movable) {
				go->Position += movable->Velocity * ElapsedSeconds;
				movable->moved = true;
			}
			
		}

		void Move(std::weak_ptr<GameObject> go, float ElapsedSeconds)
		{
			std::shared_ptr<GameObject> temp = go.lock();
			if (temp) {
				MovableComponent* movable = static_cast<MovableComponent*>(temp->GetComponent("MovableComponent"));
				if (movable) {
					temp->Position += movable->Velocity * ElapsedSeconds;
					movable->moved = true;
				}
			}
		}


		void UpdateAll(_LARGE_INTEGER currTime) {
			for (auto it = AllMovables.begin(); it != AllMovables.end(); ) {
				std::shared_ptr<GameObject> temp = it->lock();
				if (temp) {
					
					// the game object exists
					MovableComponent* movable = static_cast<MovableComponent*>(temp->GetComponent("MovableComponent"));
					
					// if there is movable component
					if (movable) {
						// if the object has not been moved yet
						if (!movable->moved) {

							// move it
							_LARGE_INTEGER ElapsedTime;
							ElapsedTime.QuadPart = currTime.QuadPart - Engine::TimeSystem::GetLastRecordedTime().QuadPart;
							float ElapsedSeconds = static_cast<float>(ElapsedTime.QuadPart) / Engine::TimeSystem::GetFrequency().QuadPart;

							Move(temp, ElapsedSeconds);
						}
						movable->moved = false;
						++it;
					}
					// if there is NOT movable component
					else {
						it = AllMovables.erase(it); // erase it from list
						continue;
					}

					
				}

				// the game object NO LONGER exists
				else {
					it = AllMovables.erase(it); // erase it from list
				}
			}
		}

		void ReleaseMovableList() {
			std::vector<std::weak_ptr<GameObject>>().swap(AllMovables);
		}

		void SetVelocity(std::shared_ptr<GameObject> go, Point2D velocity)
		{
			MovableComponent* movable = static_cast<MovableComponent*>(go->GetComponent("MovableComponent"));
			if (movable) {
				movable->Velocity = velocity;
			}
		}

		void ClearAllMovedStatus()
		{
			for (auto it = AllMovables.begin(); it != AllMovables.end(); ) {
				std::shared_ptr<GameObject> temp = it->lock();
				if (temp) {
					MovableComponent* movable = static_cast<MovableComponent*>(temp->GetComponent("MovableComponent"));
					if (movable) {
						movable->moved = false;
					}
					++it;
				}
				else {
					it = AllMovables.erase(it);
				}
			}
		}

		void SetMovedStatus(std::weak_ptr<GameObject> go, bool status)
		{
			std::shared_ptr<GameObject> temp = go.lock();
			if (temp) {
				MovableComponent* movable = static_cast<MovableComponent*>(temp->GetComponent("MovableComponent"));
				if (movable) {
					movable->moved = status;
				}
			}
			
		}

		void SetMovedStatus(MovableComponent* movable, bool status)
		{
			movable->moved = status;
		}
	}
}


void Engine::MovableSystem::Init()
{
	Engine::GameObjectFactory::RegisterComponentCreatorFunc("moveable", Engine::MovableSystem::CreateMoveableFromJSON);
};


void Engine::MovableSystem::CreateMoveableFromJSON(std::shared_ptr<GameObject> go, nlohmann::json& jsonData)
{
	assert(jsonData["velocity"].is_array());
	assert(jsonData["acceleration"].is_array());

	Engine::MovableSystem::CreateMovableComponent(go,
		Point2D{ jsonData["velocity"][0], jsonData["velocity"][1] },
		Point2D{ jsonData["acceleration"][0], jsonData["acceleration"][1] });
}

void Engine::MovableSystem::CreateMovableComponent(std::shared_ptr<GameObject> go, Point2D velocity, Point2D acceleration)
{
	MovableComponent* movable = new MovableComponent;
	movable->Velocity = velocity;
	movable->Acceleration = acceleration;
	go->AddComponent("MovableComponent", movable);
	AllMovables.push_back(go);
};