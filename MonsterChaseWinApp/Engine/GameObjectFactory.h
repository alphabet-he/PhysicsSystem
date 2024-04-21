#pragma once

#include "GameObject.h"

#include "nlohmann/json.hpp"

#include <memory>

namespace Engine 
{

	namespace GameObjectFactory 
	{

		std::shared_ptr<GameObject> CreateGameObject(const char* i_pJSONFileAddr);

		void RegisterControllerCreatorFunc(const std::string& i_ControllerType, std::function<void(std::shared_ptr<GameObject>, nlohmann::json&)> i_ControllerCreatorFunc);
		void RegisterComponentCreatorFunc(const std::string& i_ComponentType, std::function<void(std::shared_ptr<GameObject>, nlohmann::json&)> i_ComponentCreatorFunc);
	}
}

