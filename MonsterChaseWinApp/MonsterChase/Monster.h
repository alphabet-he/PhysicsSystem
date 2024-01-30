#pragma once
#include "Actor.h"

class Monster : public Actor
{
public:
	int ind;
	bool moveX = true;

	Monster();
	Monster(char* nn, int xx, int yy, int i);
	Monster(int xx, int yy, int i);

	bool chase(int targx, int targy);

	void printInfo();
};

