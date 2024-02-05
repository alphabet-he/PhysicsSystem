#pragma once
#include "GameObject.h"
#include "MovableComponent.h"

class MovableSystem
{
public:
	void CreateMovableComponent(GameObject* go, Point2D Velocity, Point2D Acceleration);
};

