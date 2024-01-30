#pragma once
#include "Point2D.h"
#include <iostream>
class Actor
{
protected:
	char* name;
	Point2D loc;

public:

	Actor(char* n, Point2D l) : loc(l)
	{
		name = n;
	};

	char* getName() { return name; }
	Point2D getLoc() { return loc; }

	virtual void printInfo() { printf("Print actor class"); };

	//virtual bool chase(int targx, int targy);
};

