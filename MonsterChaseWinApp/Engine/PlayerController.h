#pragma once
#include <string>
#include "GameObject.h"
#include <memory>


class PlayerController 
{
public:
	PlayerController();
	PlayerController(std::shared_ptr<GameObject> go, std::string name);

private:
	void PlayerKeyCallback(unsigned int i_VKeyID, bool bWentDown);

public:
	std::string PlayerName = "";

	std::shared_ptr<GameObject> PlayerObject = nullptr;


};
