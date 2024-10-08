#include "RenderSystem.h"
#include <cassert>
#include "LoadFileToBuffer.h"

namespace Engine {
    namespace RenderSystem {

        std::vector<std::weak_ptr<GameObject>> AllRenderables;


        void Init() {
            GameObjectFactory::RegisterComponentCreatorFunc("render", CreateRenderFromJSON);
        }

        void CreateRenderFromJSON(std::shared_ptr<GameObject> gameObject, nlohmann::json& jsonData)
        {
            assert(jsonData["sprite_texture_source"].is_string());
            std::string TextureSource = jsonData["sprite_texture_source"];
            CreateRenderComponent(gameObject, TextureSource.c_str());
        };


        void RenderAll()
        {

            for (auto it = AllRenderables.begin(); it != AllRenderables.end(); ) {
                std::shared_ptr<GameObject> temp = it->lock();
                if (temp) {
                    Render(temp);
                    ++it;
                }
                else {
                    it = AllRenderables.erase(it); 
                }
            }
            
        }

        void Render(std::shared_ptr<GameObject> go)
        {
            RenderComponent* render = static_cast<RenderComponent*>(go->GetComponent("RenderComponent"));
            if (render != nullptr) {
                GLib::Render(*(render->Sprite), go->Position.ToGlibPoint2D(), 0.0f, go->Rotation.z);
                render->Sprite;
            }

        }

        void CreateRenderComponent(std::shared_ptr<GameObject> go, const char* pFilename)
        {
            RenderComponent* render = new RenderComponent;
            render->Sprite = Engine::RenderSystem::CreateSprite(pFilename);
            go->AddComponent("RenderComponent", render);
            AllRenderables.push_back(go);
        }

        void ReleaseAll() {
            for (std::weak_ptr<GameObject> Current : AllRenderables) {
                std::shared_ptr<GameObject> temp = Current.lock();
                if (temp) {
                    RenderComponent* render = static_cast<RenderComponent*>(temp->GetComponent("RenderComponent"));
                    if (render != nullptr) {
                        GLib::Release(render->Sprite);
                    }
                }
                
            }

            std::vector<std::weak_ptr<GameObject>>().swap(AllRenderables);
        }

        GLib::Sprite* CreateSprite(const char* i_pFilename)
        {
            assert(i_pFilename);

            size_t sizeTextureFile = 0;

            // Load the source file (texture data)
            std::vector<uint8_t> TextureData = LoadFileToBuffer(i_pFilename);

            GLib::Sprite* pSprite = nullptr;

            // Ask GLib to create a texture out of the data (assuming it was loaded successfully)
            if (!TextureData.empty())
            {
                GLib::Texture* pTexture = GLib::CreateTexture(TextureData.data(), TextureData.size());

                unsigned int width = 0;
                unsigned int height = 0;
                unsigned int depth = 0;

                // Get the dimensions of the texture. We'll use this to determine how big it is on screen
                bool result = GLib::GetDimensions(*pTexture, width, height, depth);
                assert(result == true);
                assert((width > 0) && (height > 0));

                // Define the sprite edges
                GLib::SpriteEdges	Edges = { -float(width / 2.0f), float(height), float(width / 2.0f), 0.0f };
                GLib::SpriteUVs	UVs = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } };
                GLib::RGBA							Color = { 255, 255, 255, 255 };

                // Create the sprite
                pSprite = GLib::CreateSprite(Edges, 0.1f, Color, UVs, pTexture);

                // release our reference on the Texture
                GLib::Release(pTexture);
            }

            return pSprite;
        }
    }

}




