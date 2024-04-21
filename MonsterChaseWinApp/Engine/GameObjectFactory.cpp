#include "GameObjectFactory.h"
#include "LoadFileToBuffer.h"

namespace Engine {

	namespace GameObjectFactory {

		std::map<std::string, std::function<void(std::shared_ptr<GameObject>, nlohmann::json&)> > ControllerCreators;
		std::map<std::string, std::function<void(std::shared_ptr<GameObject>, nlohmann::json&)> > ComponentCreators;

		std::shared_ptr<GameObject> CreateGameObject(const char* i_pJSONFileAddr)
		{
			using json = nlohmann::json;

			std::vector<uint8_t> JsonDataRaw = LoadFileToBuffer(i_pJSONFileAddr);

			// if json file is empty
			if (JsonDataRaw.empty()) {
				return nullptr;
			}

			// json file not empty
			else {

				// parse json data
				json jsonData = json::parse(JsonDataRaw);

				// create default game object
				std::shared_ptr<GameObject> gameObject = GameObject::CreateGameObject();

				// if there is a name
				if (jsonData.contains("name")) {

					assert(jsonData["name"].is_string());
					gameObject->Name = jsonData["name"];
				}

				// if there is a position
				if (jsonData.contains("position")) {

					assert(jsonData["position"].is_array());
					gameObject->Position = Point2D{ jsonData["position"][0] , jsonData["position"][1] };

				}

				// if there is controller
				if (jsonData.contains("controller")) {

					assert(jsonData["controller"].is_object());
					assert(jsonData["controller"]["type"].is_string());
					assert(jsonData["controller"]["initializer"].is_object());

					// find the creator based on the controller type
					auto ControllerCreatorFunc = ControllerCreators.find(jsonData["controller"]["type"]);
					if (ControllerCreatorFunc != ControllerCreators.end()) {
						// call creator function
						ControllerCreatorFunc->second(gameObject, jsonData["controller"]["initializer"]);
					}
				}

				// if there are components
				if (jsonData.contains("components")) {
					assert(jsonData["components"].is_object());

					// iterate through all components
					for (json::iterator it = jsonData["components"].begin(); it != jsonData["components"].end(); it++) {

						assert(it->is_object());

						// find the current component type and find the creator function
						auto ComponentCreatorFunc = ComponentCreators.find(it.key());
						if (ComponentCreatorFunc != ComponentCreators.end()) {
							// call the creator function
							ComponentCreatorFunc->second(gameObject, it.value());
						}
					}
				}

				return gameObject;
			}
		}

		void RegisterControllerCreatorFunc(const std::string& i_ControllerType, std::function<void(std::shared_ptr<GameObject>, nlohmann::json&)> i_ControllerCreatorFunc)
		{
			ControllerCreators.insert({ i_ControllerType, i_ControllerCreatorFunc });
		}

		void RegisterComponentCreatorFunc(const std::string& i_ComponentType, std::function<void(std::shared_ptr<GameObject>, nlohmann::json&)> i_ComponentCreatorFunc)
		{
			ComponentCreators.insert({ i_ComponentType, i_ComponentCreatorFunc });
		}

	}

}


