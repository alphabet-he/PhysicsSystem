#pragma once
#include "Actor.h"
class Node
{
    public:
        Actor* value;
        Node* next;

        // Parameterised Constructor
        Node(Actor* a) : value(a)
        {
            this->value = a;
            this->next = nullptr;
        }
};

