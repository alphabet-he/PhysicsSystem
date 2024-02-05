#pragma once

#include <string>
#include <map>
#include "Point2D.h"

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

private:
	std::map<std::string, void* > Components;
};