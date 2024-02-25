#pragma once
#include "GameObject.h"
#include "RenderComponent.h"
#include <GLib.h>
#include <vector>
#include "nlohmann/json.hpp"
#include "GameObjectFactory.h"

namespace Engine 
{
	namespace RenderSystem {

		void Init();

		void CreateRenderFromJSON(GameObject& gameObject, nlohmann::json& jsonData);

		void RenderAll();
		void Render(GameObject* go);
		void CreateRenderComponent(GameObject* go, const char* pFilename);
		void ReleaseAll();

		GLib::Sprite* CreateSprite(const char* i_pFilename);
	}

}


