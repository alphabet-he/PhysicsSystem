#include "GameObjectControllerSystem.h"
#include <map>
#include <string>


namespace Engine 
{
	namespace GameObjectController {

		// list of all game object controllers
		std::map<std::string, PlayerController*> PlayerControllerList;
		// might be AIs, I'm not sure

		void Init() {
			GameObjectFactory::RegisterControllerCreatorFunc("player", CreatePlayerController);
		}

		void AddToPlayerControllerList(PlayerController* pc)
		{
			PlayerControllerList.insert({ pc->PlayerName, pc });
		}

		void CreatePlayerController(std::shared_ptr<GameObject> gameObject, nlohmann::json& jsonData)
		{
			assert(jsonData["name"].is_string());
			PlayerController* pc = new PlayerController(gameObject, jsonData["name"]);
			PlayerControllerList.insert({ jsonData["name"], pc });
		}

		void ReleaseAll()
		{
			for (auto it = PlayerControllerList.begin(); it != PlayerControllerList.end(); it++) {
				it->second->Release();
				delete it->second;
			}
		}

	}

}
