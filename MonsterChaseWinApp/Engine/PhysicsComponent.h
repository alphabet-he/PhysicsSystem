#pragma once
#include "Point2D.h"


struct PhysicsComponent
{
	Point2D kDrag; //kg * m * s^-2
	float Mass; //kg
};