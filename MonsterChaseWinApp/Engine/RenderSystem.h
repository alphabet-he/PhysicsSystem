#pragma once
#include "GameObject.h"
#include "RenderComponent.h"
#include "FunctionLib.h"
#include <GLib.h>
#include <vector>


class RenderSystem
{
public:
	void RenderAll();
	void Render(GameObject* go);
	void CreateRenderComponent(GameObject* go, const char* pFilename);
	void ReleaseAll();

private:
	std::vector<GameObject*> AllRenderables;
	GLib::Sprite* CreateSprite(const char* i_pFilename);
};

