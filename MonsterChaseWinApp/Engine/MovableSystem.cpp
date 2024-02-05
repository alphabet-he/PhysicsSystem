#include "MovableSystem.h"

void MovableSystem::CreateMovableComponent(GameObject* go, Point2D v, Point2D a)
{
	MovableComponent* movable = new MovableComponent;
	movable->Velocity = v;
	movable->Acceleration = a;
	go->AddComponent("MovableComponent", movable);
}
