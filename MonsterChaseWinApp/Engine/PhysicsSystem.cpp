#include "PhysicsSystem.h"

void PhysicsSystem::Update(GameObject* go, TimeSystem* GameTimeSystem)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(go->GetComponent("PhysicsComponent"));
	MovableComponent* movable = static_cast<MovableComponent*>(go->GetComponent("MovableComponent"));

	if (physics != nullptr && movable != nullptr) {
		// apply semi-implicit euler

		// find delta-t
		_LARGE_INTEGER currTime, ElapsedTime;
		float ElapsedSeconds;
		QueryPerformanceCounter(&currTime);
		ElapsedTime.QuadPart = currTime.QuadPart - GameTimeSystem->LastRecordedTime.QuadPart;
		ElapsedSeconds = static_cast<float>(ElapsedTime.QuadPart) / GameTimeSystem->Frequency.QuadPart;


		// update velocity
		movable->Velocity += movable->Acceleration * ElapsedSeconds;

		// update position
		go->Position += movable->Velocity * ElapsedSeconds;

		// update acceleration
		movable->Acceleration = physics->kDrag / physics->Mass;

	}
}

void PhysicsSystem::CreatePhysicsComponent(GameObject* go, Point2D kDrag, float mass)
{
	PhysicsComponent* physics = new PhysicsComponent;
	physics->kDrag = kDrag;
	physics->Mass = mass;
	go->AddComponent("PhysicsComponent", physics);
}

void PhysicsSystem::AddForce(GameObject* go, Point2D force)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(go->GetComponent("PhysicsComponent"));
	physics->kDrag += force;
}

void PhysicsSystem::ReleaseXForce(GameObject* go)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(go->GetComponent("PhysicsComponent"));
	physics->kDrag.x = 0;
}

void PhysicsSystem::ReleaseYForce(GameObject* go)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(go->GetComponent("PhysicsComponent"));
	physics->kDrag.y = 0;
}


