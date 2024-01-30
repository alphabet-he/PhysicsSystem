#include "Monster.h"
#include <iostream>

bool Monster::chase(int targx, int targy) {

	bool moved = false;
	//int targx = targ.x;
	//int targy = targ.y;
	if (moveX) { // check to move X first
		if (loc.x < targx) { loc.x++; moved = true; }
		if (loc.x > targx) { loc.x--; moved = true; }
		moveX = false;
	}
	else if (!moveX && !moved) {
		if (loc.y < targy) loc.y++;
		if (loc.y > targy) loc.y--;
		moveX = true;
	}
	if (loc.x == targx && loc.y == targy) {
		
		return true;
	}
	return false;

}

void Monster::printInfo()
{
	printf("\n%s is at [%d, %d]",
		    name,
		    loc.x,
		    loc.y);
}

Monster::Monster(): Actor(name, loc)
{
	name = nullptr;
	loc.x = -1;
	loc.y = -1;
	ind = -1;
}

Monster::Monster(char* nn, int xx, int yy, int i): Actor(name, loc)
{
	name = nn;
	loc.x = xx;
	loc.y = yy;
	ind = i;
}

Monster::Monster(int xx, int yy, int i) : Actor(name, loc)
{
	name = nullptr;
	loc.x = xx;
	loc.y = yy;
	ind = i;
}
