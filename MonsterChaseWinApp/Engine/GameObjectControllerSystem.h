#pragma once
#include "PlayerController.h"
#include "nlohmann/json.hpp"
#include "GameObjectFactory.h"

namespace Engine 
{
	namespace GameObjectController 
	{
		void Init();

		void setQuit(bool value);

		bool getQuit();

		void AddToPlayerControllerList(PlayerController* pc);

		void CreatePlayerController(GameObject& gameObject, nlohmann::json& jsonData);
	}

}
