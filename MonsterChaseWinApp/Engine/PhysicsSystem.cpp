#include "PhysicsSystem.h"

void Engine::PhysicsSystem::Update(std::shared_ptr<GameObject> go)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(go->GetComponent("PhysicsComponent"));
	MovableComponent* movable = static_cast<MovableComponent*>(go->GetComponent("MovableComponent"));

	if (physics != nullptr && movable != nullptr) {
		// apply semi-implicit euler

		// find delta-t
		_LARGE_INTEGER currTime, ElapsedTime;
		float ElapsedSeconds;
		QueryPerformanceCounter(&currTime);
		ElapsedTime.QuadPart = currTime.QuadPart - Engine::TimeSystem::GetLastRecordedTime().QuadPart;
		ElapsedSeconds = static_cast<float>(ElapsedTime.QuadPart) / Engine::TimeSystem::GetFrequency().QuadPart;


		// update velocity
		movable->Velocity += movable->Acceleration * ElapsedSeconds;

		// update position
		go->Position += movable->Velocity * ElapsedSeconds;

		// update acceleration
		movable->Acceleration = physics->kDrag / physics->Mass;

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

