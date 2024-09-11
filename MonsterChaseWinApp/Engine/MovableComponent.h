#pragma once

struct MovableComponent
{
	Point2D Velocity; // m/s
	Point2D Acceleration;  //m/s^2

	bool moved = false;
};
