#pragma once
#include <string>
#include "GameObject.h"


class PlayerController 
{
public:
	PlayerController();
	PlayerController(GameObject* go, std::string name);

private:
	void PlayerKeyCallback(unsigned int i_VKeyID, bool bWentDown);

public:
	std::string PlayerName = "";

	GameObject* PlayerObject = nullptr;


};
