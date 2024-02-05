#pragma once
#include "GameObject.h"
#include "MovableComponent.h"

class MovableSystem
{
public:
	inline void CreateMovableComponent(GameObject* go, Point2D velocity, Point2D acceleration) const {
		MovableComponent* movable = new MovableComponent;
		movable->Velocity = velocity;
		movable->Acceleration = acceleration;
		go->AddComponent("MovableComponent", movable);
	};
};

