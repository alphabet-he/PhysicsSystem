#pragma once
#include <string>
#include "GameObject.h"
#include <memory>


class PlayerController 
{
public:
	static std::shared_ptr<PlayerController> CreatePlayerController() {
		return std::make_shared<PlayerController>(PlayerController());
	}

	static std::shared_ptr<PlayerController> CreatePlayerController(std::shared_ptr<GameObject> go, std::string name) {
		return std::make_shared<PlayerController>(PlayerController(go, name));
	}

	PlayerController();
	PlayerController(std::shared_ptr<GameObject> go, std::string name);

	void Release() {
		PlayerObject = nullptr;
	}

private:
	void PlayerKeyCallback(unsigned int i_VKeyID, bool bWentDown);

public:
	std::string PlayerName = "";

	std::shared_ptr<GameObject> PlayerObject;


};
