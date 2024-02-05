#include "PhysicsSystem.h"

void PhysicsSystem::Update(GameObject* go, TimeSystem* GameTimeSystem) const
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





