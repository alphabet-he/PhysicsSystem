#pragma once
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "MovableComponent.h"
#include "TimeSystem.h"
#include "json.hpp"
#include "GameObjectFactory.h"
#include <memory>

namespace Engine {

	namespace PhysicsSystem {

		void Init();
		
		void Update(std::shared_ptr<GameObject> go);

		void CreatePhysicsFromJSON(std::shared_ptr<GameObject> gameObject, nlohmann::json& jsonData);

		void CreatePhysicsComponent(std::shared_ptr<GameObject> go, Point2D kDrag, float mass);

		
		void AddForce(std::shared_ptr<GameObject> go, Point2D force);

		void ReleaseXForce(std::shared_ptr<GameObject> go);
		void ReleaseYForce(std::shared_ptr<GameObject> go);
		
	}

}

