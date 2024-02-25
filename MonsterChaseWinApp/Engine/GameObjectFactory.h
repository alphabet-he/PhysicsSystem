#pragma once

#include "GameObject.h"

#include "nlohmann/json.hpp"

namespace Engine 
{

	namespace GameObjectFactory 
	{

		std::map<std::string, std::function<void(GameObject&, nlohmann::json&)> > ControllerCreators;
		std::map<std::string, std::function<void(GameObject&, nlohmann::json&)> > ComponentCreators;


		GameObject* CreateGameObject(const char* i_pJSONFileAddr);

		void RegisterControllerCreatorFunc(const std::string& i_ControllerType, std::function<void(GameObject&, nlohmann::json&)> i_ControllerCreatorFunc);
		void RegisterComponentCreatorFunc(const std::string& i_ComponentType, std::function<void(GameObject&, nlohmann::json&)> i_ComponentCreatorFunc);
	}
}

