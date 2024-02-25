#pragma once
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "MovableComponent.h"
#include "TimeSystem.h"
#include "nlohmann/json.hpp"
#include "GameObjectFactory.h"

namespace Engine {

	namespace PhysicsSystem {

		void Init();
		
		void Update(GameObject* go);

		void CreatePhysicsFromJSON(GameObject& gameObject, nlohmann::json& jsonData);

		void CreatePhysicsComponent(GameObject& go, Point2D kDrag, float mass);

		
		void AddForce(GameObject* go, Point2D force);

		void ReleaseXForce(GameObject* go);
		void ReleaseYForce(GameObject* go);
		
	}

}

