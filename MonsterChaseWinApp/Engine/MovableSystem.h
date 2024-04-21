#pragma once
#include "GameObject.h"
#include "MovableComponent.h"
#include "nlohmann/json.hpp"
#include "GameObjectFactory.h"
#include <memory>


namespace Engine {

	namespace MovableSystem {

		void Init();


		void CreateMoveableFromJSON(std::shared_ptr<GameObject> go, nlohmann::json& jsonData);

		void CreateMovableComponent(std::shared_ptr<GameObject> go, Point2D velocity, Point2D acceleration);
	}
}


