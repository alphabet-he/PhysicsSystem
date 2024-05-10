#pragma once
#include "GameObject.h"
#include "MovableComponent.h"
#include "json.hpp"
#include "GameObjectFactory.h"
#include <memory>


namespace Engine {

	namespace MovableSystem {

		void Init();


		void CreateMoveableFromJSON(std::shared_ptr<GameObject> go, nlohmann::json& jsonData);

		void CreateMovableComponent(std::shared_ptr<GameObject> go, Point2D velocity, Point2D acceleration);

		void UpdateAll(_LARGE_INTEGER currTime);

		void Update(std::shared_ptr<GameObject> go, _LARGE_INTEGER currTime);

		void Move(std::weak_ptr<GameObject> go, float ElapsedSeconds);

		void ReleaseMovableList();

		void SetVelocity(std::shared_ptr<GameObject> go, Point2D velocity);

		void ClearAllMovedStatus();

		void SetMovedStatus(std::weak_ptr<GameObject> go, bool status);

		void SetMovedStatus(MovableComponent* movable, bool status);
	
	}
}


