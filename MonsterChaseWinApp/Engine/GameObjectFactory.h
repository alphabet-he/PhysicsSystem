#pragma once

#include "GameObject.h"

#include "nlohmann/json.hpp"

namespace Engine 
{

	namespace GameObjectFactory 
	{

		GameObject* CreateGameObject(const char* i_pJSONFileAddr);

		void RegisterControllerCreatorFunc(const std::string& i_ControllerType, std::function<void(GameObject&, nlohmann::json&)> i_ControllerCreatorFunc);
		void RegisterComponentCreatorFunc(const std::string& i_ComponentType, std::function<void(GameObject&, nlohmann::json&)> i_ComponentCreatorFunc);
	}
}

