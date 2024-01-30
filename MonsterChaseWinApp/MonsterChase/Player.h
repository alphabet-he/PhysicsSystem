#pragma once
#include <Actor.h>
class Player :
    public Actor
{

public:
    Player(char* n, Point2D l) : Actor(name, loc) 
    {
        name = n;
        loc = l;
    };
};

