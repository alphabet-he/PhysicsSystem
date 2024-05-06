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
			}
			
		}

		void UpdateAll(_LARGE_INTEGER currTime) {
			for (auto it = AllMovables.begin(); it != AllMovables.end(); ) {
				std::shared_ptr<GameObject> temp = it->lock();
				if (temp) {
					Update(temp, currTime);
					++it;
				}
				else {
					it = AllMovables.erase(it);
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