#pragma once
#include "GameObject.h"
#include "RenderComponent.h"
#include <GLib.h>
#include <vector>
#include "nlohmann/json.hpp"
#include "GameObjectFactory.h"
#include <memory>

namespace Engine 
{
	namespace RenderSystem {

		void Init();

		void CreateRenderFromJSON(std::shared_ptr<GameObject> gameObject, nlohmann::json& jsonData);

		void RenderAll();
		void Render(std::shared_ptr<GameObject> go);
		void CreateRenderComponent(std::shared_ptr<GameObject> go, const char* pFilename);
		void ReleaseAll();

		GLib::Sprite* CreateSprite(const char* i_pFilename);
	}

}


