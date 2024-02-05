#include "RenderSystem.h"
#include <cassert>

void RenderSystem::RenderAll()
{
	for (GameObject* Current : AllRenderables) {
		Render(Current);
	}
}

void RenderSystem::Render(GameObject* go)
{
	RenderComponent* render = static_cast<RenderComponent*>(go->GetComponent("RenderComponent"));
	if (render != nullptr) {
		GLib::Render(*(render->Sprite), go->Position.ToGlibPoint2D(), 0.0f, 0.0f);
	}
		
}

void RenderSystem::CreateRenderComponent(GameObject* go, const char* pFilename)
{
	RenderComponent* render = new RenderComponent;
	render->i_pFilename = pFilename;
    render->Sprite = CreateSprite(pFilename);
    go->AddComponent("RenderComponent", render);
    AllRenderables.push_back(go);
}

void RenderSystem::ReleaseAll() {
    for (GameObject* Current : AllRenderables) {
        RenderComponent* render = static_cast<RenderComponent*>(Current->GetComponent("RenderComponent"));
        if (render != nullptr) {
            GLib::Release(render->Sprite);
        }
    }
}

GLib::Sprite* RenderSystem::CreateSprite(const char* i_pFilename)
{
    assert(i_pFilename);

    size_t sizeTextureFile = 0;

    // Load the source file (texture data)
    void* pTextureFile = LoadFile(i_pFilename, sizeTextureFile);

    // Ask GLib to create a texture out of the data (assuming it was loaded successfully)
    GLib::Texture* pTexture = pTextureFile ? GLib::CreateTexture(pTextureFile, sizeTextureFile) : nullptr;

    // exit if something didn't work
    // probably need some debug logging in here!!!!
    if (pTextureFile)
        delete[] pTextureFile;

    if (pTexture == nullptr)
        return nullptr;

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
    GLib::Sprite* pSprite = GLib::CreateSprite(Edges, 0.1f, Color, UVs, pTexture);

    // release our reference on the Texture
    GLib::Release(pTexture);

    return pSprite;
}



