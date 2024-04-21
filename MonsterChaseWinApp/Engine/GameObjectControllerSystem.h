#pragma once
#include "PlayerController.h"
#include "nlohmann/json.hpp"
#include "GameObjectFactory.h"
#include <memory>

namespace Engine 
{
	namespace GameObjectController 
	{
		void Init();

		void AddToPlayerControllerList(PlayerController* pc);

		void CreatePlayerController(std::shared_ptr<GameObject> gameObject, nlohmann::json& jsonData);

		void ReleaseAll();
	}

}
