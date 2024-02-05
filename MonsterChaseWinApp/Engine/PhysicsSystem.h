#pragma once
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "MovableComponent.h"
#include "TimeSystem.h"

class PhysicsSystem {

public:
	void Update(GameObject* go, TimeSystem* GameTimeSystem);
	void CreatePhysicsComponent(GameObject* go, Point2D kDrag, float mass);
	void AddForce(GameObject* go, Point2D force);
	void ReleaseXForce(GameObject* go);
	void ReleaseYForce(GameObject* go);
};