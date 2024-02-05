#pragma once
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "MovableComponent.h"
#include "TimeSystem.h"

class PhysicsSystem {

public:
	void Update(GameObject* go, TimeSystem* GameTimeSystem) const;


	void CreatePhysicsComponent(GameObject* go, Point2D kDrag, float mass) const {
		PhysicsComponent* physics = new PhysicsComponent;
		physics->kDrag = kDrag;
		physics->Mass = mass;
		go->AddComponent("PhysicsComponent", physics);
	};

	inline void AddForce(GameObject* go, Point2D force) const {
		PhysicsComponent* physics = static_cast<PhysicsComponent*>(go->GetComponent("PhysicsComponent"));
		physics->kDrag += force;
	};

	inline void ReleaseXForce(GameObject* go) const {
		PhysicsComponent* physics = static_cast<PhysicsComponent*>(go->GetComponent("PhysicsComponent"));
		physics->kDrag.x = 0;
	};
	void ReleaseYForce(GameObject* go) const {
		PhysicsComponent* physics = static_cast<PhysicsComponent*>(go->GetComponent("PhysicsComponent"));
		physics->kDrag.y = 0;
	};
};