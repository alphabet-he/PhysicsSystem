#pragma once
#include "GameObject.h"
#include "CollisionComponent.h"
#include "json.hpp"
#include "GameObjectFactory.h"
#include <memory>

namespace Engine {

	namespace CollisionSystem {

		void Init();

		void CreateCollisionFromJSON(std::shared_ptr<GameObject> go, nlohmann::json& jsonData);

		void CreateCollisionComponent(std::shared_ptr<GameObject> go, Point2D velocity, Point2D acceleration);

		void ReleaseCollidableList();
	}
}
