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
			float collisionTime = -1.0f; // from the time arriving at current position to the time of collision

			void copy(CollisionInfo other) {
				objA = other.objA;
				objB = other.objB;
				collisionTime = other.collisionTime;
			}
		};

		void Init();

		void CreateCollisionFromJSON(std::shared_ptr<GameObject> go, nlohmann::json& jsonData);

		void CreateCollisionComponent(std::shared_ptr<GameObject> go, Point2D velocity, Point2D acceleration);

		void ReleaseCollidableList();

		void CheckAllCollision(_LARGE_INTEGER currTime);

		CollisionInfo DetectCollision(std::weak_ptr<GameObject> objA, std::weak_ptr<GameObject> objB, 
			float currTimeInFrame, float frameEndTime);

		bool CalculateCloseOpenTime(CollisionComponent* currCollision, CollisionComponent* targetCollision,
			Matrix4 matrixCurrToTarget, Vector4 velInTargetCs,
			std::vector<float>& t_close, std::vector<float>& t_open,
			float leftFrameTime);

		Point2D GetVelocity(std::shared_ptr<GameObject> obj);

		void TrimCollidableList();

		void UpdateVelocityAfterCollision(std::weak_ptr<GameObject> objA, std::weak_ptr<GameObject> objB);
	}
}
