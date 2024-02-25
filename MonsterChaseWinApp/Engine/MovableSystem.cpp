#include "MovableSystem.h"

void Engine::MovableSystem::Init()
{
	Engine::GameObjectFactory::RegisterComponentCreatorFunc("moveable", Engine::MovableSystem::CreateMoveableFromJSON);
};


void Engine::MovableSystem::CreateMoveableFromJSON(GameObject& go, nlohmann::json& jsonData)
{
	assert(jsonData["velocity"].is_array());
	assert(jsonData["acceleration"].is_array());

	Engine::MovableSystem::CreateMovableComponent(go,
		Point2D{ jsonData["velocity"][0], jsonData["velocity"][1] },
		Point2D{ jsonData["acceleration"][0], jsonData["acceleration"][1] });
}

void Engine::MovableSystem::CreateMovableComponent(GameObject& go, Point2D velocity, Point2D acceleration)
{
	MovableComponent* movable = new MovableComponent;
	movable->Velocity = velocity;
	movable->Acceleration = acceleration;
	go.AddComponent("MovableComponent", movable);
};