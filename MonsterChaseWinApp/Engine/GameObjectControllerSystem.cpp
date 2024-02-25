#include "GameObjectControllerSystem.h"
#include <map>
#include <string>


namespace Engine 
{
	namespace GameObjectController {

		// quit boolean
		bool g_quit = false;

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

		void CreatePlayerController(GameObject& gameObject, nlohmann::json& jsonData)
		{
			assert(jsonData["name"].is_string());
			PlayerController* pc = new PlayerController(&gameObject, jsonData["name"]);
			PlayerControllerList.insert({ jsonData["name"], pc });
		}

		void setQuit(bool value)
		{
			g_quit = value;
		}
		bool& getQuit()
		{
			return g_quit;
		}
	}

}
