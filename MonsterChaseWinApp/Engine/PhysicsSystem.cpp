#include "PhysicsSystem.h"

namespace Engine {
	namespace PhysicsSystem {
		std::vector<std::weak_ptr<GameObject>> AllPhysics;

		void Update(std::shared_ptr<GameObject> go, _LARGE_INTEGER currTime)
		{
			_LARGE_INTEGER ElapsedTime;
			ElapsedTime.QuadPart = currTime.QuadPart - Engine::TimeSystem::GetLastRecordedTime().QuadPart;
			float ElapsedSeconds = static_cast<float>(ElapsedTime.QuadPart) / Engine::TimeSystem::GetFrequency().QuadPart;


			PhysicsComponent* physics = static_cast<PhysicsComponent*>(go->GetComponent("PhysicsComponent"));
			MovableComponent* movable = static_cast<MovableComponent*>(go->GetComponent("MovableComponent"));

			if (physics != nullptr && movable != nullptr) {
				// apply semi-implicit euler

				// update velocity
				movable->Velocity += movable->Acceleration * ElapsedSeconds;

				// update acceleration
				movable->Acceleration = physics->kDrag / physics->Mass;

			}
		}

		void UpdateAll(_LARGE_INTEGER currTime) {
			for (auto it = AllPhysics.begin(); it != AllPhysics.end(); ) {
				std::shared_ptr<GameObject> temp = it->lock();
				if (temp) {
					Update(temp, currTime);
					++it;
				}
				else {
					it = AllPhysics.erase(it);
				}
			}
		}

		void ReleasePhysicsList() {
			std::vector<std::weak_ptr<GameObject>>().swap(AllPhysics);
		}
	}
}



void Engine::PhysicsSystem::Init()
{
	Engine::GameObjectFactory::RegisterComponentCreatorFunc("physics", Engine::PhysicsSystem::CreatePhysicsFromJSON);
}


void Engine::PhysicsSystem::CreatePhysicsFromJSON(std::shared_ptr<GameObject> gameObject, nlohmann::json& jsonData)
{
	assert(jsonData["kDrag"].is_array());
	assert(jsonData["mass"].is_number_float());
	Engine::PhysicsSystem::CreatePhysicsComponent(gameObject, Point2D{ jsonData["kDrag"][0], jsonData["kDrag"][1] }, jsonData["mass"]);
}

void Engine::PhysicsSystem::CreatePhysicsComponent(std::shared_ptr<GameObject> go, Point2D kDrag, float mass) {
	PhysicsComponent* physics = new PhysicsComponent;
	physics->kDrag = kDrag;
	physics->Mass = mass;
	go->AddComponent("PhysicsComponent", physics);
	AllPhysics.push_back(go);
}


void Engine::PhysicsSystem::AddForce(std::shared_ptr<GameObject> go, Point2D force) {
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(go->GetComponent("PhysicsComponent"));
	physics->kDrag += force;
}

void Engine::PhysicsSystem::ReleaseXForce(std::shared_ptr<GameObject> go) {
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(go->GetComponent("PhysicsComponent"));
	physics->kDrag.x = 0;
}
void Engine::PhysicsSystem::ReleaseYForce(std::shared_ptr<GameObject> go) {
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(go->GetComponent("PhysicsComponent"));
	physics->kDrag.y = 0;
}

void Engine::PhysicsSystem::ReleaseForce(std::shared_ptr<GameObject> go) {
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(go->GetComponent("PhysicsComponent"));
	physics->kDrag.x = 0;
	physics->kDrag.y = 0;
}

