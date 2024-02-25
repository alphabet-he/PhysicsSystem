#include "PlayerController.h"
#include "GameObjectControllerSystem.h"
#include "PhysicsSystem.h"
#include "GameState.h"

using namespace Engine;

PlayerController::PlayerController()
{

    GLib::SetKeyStateChangeCallback([this](unsigned int i_VKeyID, bool bWentDown) {
        this->PlayerKeyCallback(i_VKeyID, bWentDown);
        });
}

PlayerController::PlayerController(GameObject* go, std::string name)
{
	PlayerObject = go;
	PlayerName = name;
	
    GLib::SetKeyStateChangeCallback([this](unsigned int i_VKeyID, bool bWentDown) {
        this->PlayerKeyCallback(i_VKeyID, bWentDown);
    });
}

void PlayerController::PlayerKeyCallback(unsigned int i_VKeyID, bool bWentDown)
{
    if (!PlayerObject) {
        return;
    }

    if (i_VKeyID == 'Q' && bWentDown)
    {
        GameState::setQuit(true);
    }

    if (i_VKeyID == 'W')
    {
        if (bWentDown) {
            //playerTarget.y = min(windowHeight/2, playerTarget.y + playerMoveDist);
            PhysicsSystem::AddForce(PlayerObject, Point2D({ 0, 1000 }));
        }
        else {
            PhysicsSystem::AddForce(PlayerObject, Point2D({ 0, -1000 }));
        }
    }


    if (i_VKeyID == 'A')
    {
        if (bWentDown) {
            //playerTarget.x = max(-windowWidth/2, playerTarget.x - playerMoveDist);
            PhysicsSystem::AddForce(PlayerObject, Point2D({ -10000, 0 }));
        }
        else {
            PhysicsSystem::AddForce(PlayerObject, Point2D({ 10000, 0 }));
        }

    }

    if (i_VKeyID == 'S')
    {
        if (bWentDown) {
            //playerTarget.y = max(-windowHeight/2, playerTarget.y - playerMoveDist);
            PhysicsSystem::AddForce(PlayerObject, Point2D({ 0, -10000 }));
        }
        else {
            PhysicsSystem::AddForce(PlayerObject, Point2D({ 0, 10000 }));
        }
    }

    if (i_VKeyID == 'D')
    {
        if (bWentDown) {
            //playerTarget.x = min(windowWidth / 2, playerTarget.x + playerMoveDist);
            PhysicsSystem::AddForce(PlayerObject, Point2D({ 10000, 0 }));
        }
        else {
            //playerTarget.x = min(windowWidth / 2, playerTarget.x + playerMoveDist);
            PhysicsSystem::AddForce(PlayerObject, Point2D({ -10000, 0 }));
        }

    }
}


