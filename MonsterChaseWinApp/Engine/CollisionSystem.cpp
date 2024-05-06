#include "CollisionSystem.h"

namespace Engine {
	namespace CollisionSystem {

        std::vector<std::weak_ptr<GameObject>> AllCollidables;


        void Init() {
            GameObjectFactory::RegisterComponentCreatorFunc("collision", CreateCollisionFromJSON);
        }

        void CreateCollisionFromJSON(std::shared_ptr<GameObject> gameObject, nlohmann::json& jsonData)
        {
            //assert(jsonData["sprite_texture_source"].is_string());
            //std::string TextureSource = jsonData["sprite_texture_source"];
            //CreateCollisionComponent(gameObject, TextureSource.c_str());
            assert(jsonData["center"].is_array());
            Point2D center = Point2D{ jsonData["center"][0], jsonData["center"][1] };
            assert(jsonData["extents"].is_array());
            Point2D extents = Point2D{ jsonData["extents"][0], jsonData["extents"][1] };
            CreateCollisionComponent(gameObject, center, extents);
        };

        void CreateCollisionComponent(std::shared_ptr<GameObject> go, Point2D center, Point2D extents)
        {
            //RenderComponent* render = new RenderComponent;
            //render->Sprite = Engine::RenderSystem::CreateSprite(pFilename);
            //go->AddComponent("RenderComponent", render);

            CollisionComponent* collision = new CollisionComponent;
            collision->center = center;
            collision->extents = extents;
            go->AddComponent("CollisionComponent", collision);
            AllCollidables.push_back(go);
        }

        void ReleaseCollidableList() {
            for (std::weak_ptr<GameObject> Current : AllCollidables) {
                std::shared_ptr<GameObject> temp = Current.lock();
                if (temp) {
                    CollisionComponent* collision = static_cast<CollisionComponent*>(temp->GetComponent("CollisionComponent"));
                    if (collision != nullptr) {
                        
                    }
                }

            }

            std::vector<std::weak_ptr<GameObject>>().swap(AllCollidables);
        }
	}
}
