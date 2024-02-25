#pragma once

#include <string>
#include <map>
#include "Point2D.h"
#include "PhysicsComponent.h"
#include "MovableComponent.h"
#include "RenderComponent.h"

class GameObject {


public:
	std::string Name;
	Point2D Position;
	

	GameObject() {
		Name = "";
		Position = { 0,0 };
	}

	GameObject(Point2D position) {
		Name = "";
		Position = position;
	}

	GameObject(std::string name, Point2D p) {
		Name = name;
		Position = p;
	}

	inline void* GetComponent(const std::string& ComponentName) {
		return Components.find(ComponentName) != Components.end() ? Components[ComponentName] : nullptr;
	}

	inline void AddComponent(const std::string ComponentName, void* Component) {
		Components.insert({ ComponentName, Component });
	}

	~GameObject() {
		for (auto& pair : Components) {
			delete pair.second;
			/*if (pair.first == "PhysicsComponent") {
				PhysicsComponent* comp = static_cast<PhysicsComponent*>(pair.second);
				delete comp;
			}
			if (pair.first == "MovableComponent") {
				MovableComponent* comp = static_cast<MovableComponent*>(pair.second);
				delete comp;
			}
			if (pair.first == "RenderComponent") {
				RenderComponent* comp = static_cast<RenderComponent*>(pair.second);
				delete comp;
			}*/
		}
		Components.clear();
	}

private:
	std::map<std::string, void* > Components;
};