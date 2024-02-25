#pragma once
#include "GameObject.h"
#include "MovableComponent.h"
#include "nlohmann/json.hpp"
#include "GameObjectFactory.h"


namespace Engine {

	namespace MovableSystem {

		void Init();


		void CreateMoveableFromJSON(GameObject& go, nlohmann::json& jsonData);

		void CreateMovableComponent(GameObject& go, Point2D velocity, Point2D acceleration);
	}
}


