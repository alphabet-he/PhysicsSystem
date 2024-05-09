#pragma once
#include "GameObject.h"
#include "CollisionComponent.h"
#include "json.hpp"
#include "GameObjectFactory.h"
#include <memory>
#include "Matrix4.h"

namespace Engine {

	namespace CollisionSystem {

		struct CollisionInfo {
			std::weak_ptr<GameObject> objA;
			std::weak_ptr<GameObject> objB;
			float collisionTime = -1.0f;
		};

		void Init();

		void CreateCollisionFromJSON(std::shared_ptr<GameObject> go, nlohmann::json& jsonData);

		void CreateCollisionComponent(std::shared_ptr<GameObject> go, Point2D velocity, Point2D acceleration);

		void ReleaseCollidableList();

		void CheckAllCollision();

		CollisionInfo* DetectCollision(std::weak_ptr<GameObject> objA, std::weak_ptr<GameObject> objB);

		bool CalculateCloseOpenTime(CollisionComponent* currCollision, CollisionComponent* targetCollision,
			Matrix4 matrixCurrToTarget, Vector4 velInTargetCs,
			std::vector<float>& t_close, std::vector<float>& t_open,
			float frameTime);

		Point2D GetVelocity(std::shared_ptr<GameObject> obj);

		void TrimCollidableList();
	}
}
