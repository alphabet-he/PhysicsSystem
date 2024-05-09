#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <Windows.h>

#include <ctime>
#include <memory>

#include <DirectXColors.h>

#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // _DEBUG

#include "GLib.h"
#include <GameObject.h>
#include "Point2D.h"
#include <GameState.h>

#include "Matrix4Test.h"



static int windowWidth = 800;
static int windowHeight = 600;

bool g_bQuit = false;

/*
char* enterName()
{
    char* str;
    char c = '!';
    int i = 0, j = 1;

    str = (char*)malloc(sizeof(char)); // pointer to the first char in array

    fseek(stdin, 0, SEEK_END);
    while (c != '\n') {
        // the first character
        if (c == '!') {
            c = getc(stdin);
            j++;
        }

        // re-allocate (resize) memory for character read to be stored
        str = (char*)realloc(str, j * sizeof(char));

        // store read character by making pointer point to c
        str[i] = c;

        i++;
        j++;

        // read the input from keyboard standard input
        c = getc(stdin);
    }

    str[i] = '\0'; // at the end append null character to mark end of string
    return str;
}

void printMonster(Linkedlist list) {
    Node* temp = list.head;

    // Check for empty list.
    if (temp == nullptr) {
        printf("List empty");
        return;
    }

    //printf("\n\nMonsters:");
    // Traverse the list.
    while (temp != nullptr) {
        //printf("\n%s is at [%d, %d]",
        //    temp->monster.name,
        //    temp->monster.x,
        //    temp->monster.y);
        if (Monster* m = dynamic_cast<Monster*>(temp->value))
        m->printInfo();
        temp = temp->next;
        
    }
}
*/


//void* LoadFile(const char* i_pFilename, size_t& o_sizeFile);
//GLib::Sprite* CreateSprite(const char* i_pFilename);

using namespace Engine;

void MovetoTarget(Point2D& start, Point2D target, float moveDist);

BOOL AttachOrAllocateConsole()
{
    // Try to attach to the existing console
    if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
        // If there is no console to attach to, create a new one
        if (!AllocConsole()) {
            return FALSE;  // Return FALSE if both attempts fail
        }
    }
    return TRUE;
}

void RedirectIOToConsole()
{
    FILE* pFile;

    // Redirect STDOUT to the console
    freopen_s(&pFile, "CONOUT$", "w", stdout);

    // Redirect STDIN to the console
    freopen_s(&pFile, "CONIN$", "r", stdin);

    // Redirect STDERR to the console
    freopen_s(&pFile, "CONOUT$", "w", stderr);

    // Clear the error state for each of the C++ standard streams
    std::cout.clear();
    std::cin.clear();
    std::cerr.clear();
    std::clog.clear();
}


int WINAPI wWinMain(
    HINSTANCE i_hInstance, 
    HINSTANCE i_hPrevInstance, 
    LPWSTR i_lpCmdLine, 
    int i_nCmdShow)
{
    

    // Attach or allocate a console
    if (!AttachOrAllocateConsole()) {
        MessageBox(NULL, L"Failed to attach or allocate a console!", L"Error", MB_ICONERROR);
        return 1;
    }

    // Redirect standard I/O to the console
    RedirectIOToConsole();

    // Example use of printf and std::cout
    printf("Hello, Windows GUI Application with Console Output!\n");

    if (UnitTest()) {
        std::cout << "Matrix4 unit test passed!" << std::endl;
    }
    

    // random seed
    srand((unsigned int)time(0));

    // IMPORTANT: first we need to initialize GLib
    bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "GLibTest", -1, windowWidth, windowHeight, true);

    if (bSuccess)
    {
        // IMPORTANT (if we want keypress info from GLib): Set a callback for notification of key presses
        //GLib::SetKeyStateChangeCallback(TestKeyCallback);
       
        
        // Initialize systems
        GameState::Init();

        // Create Player/Monster GameObject
        
        /*
        GameObject* Monster = new GameObject("Monster", monsterStart);
        
        GameRenderSystem->CreateRenderComponent(Player, "..\\MonsterChase\\data\\GoodGuy.dds");
        GameRenderSystem->CreateRenderComponent(Monster, "..\\MonsterChase\\data\\BadGuy.dds");
        GamePhysicsSystem->CreatePhysicsComponent(Player, Point2D{ 0, 0 }, 1.0f);
        GameMovableSystem->CreateMovableComponent(Player, Point2D{ 0,0 }, Point2D{ 0,0 });
        */
        std::shared_ptr<GameObject> Player = GameObjectFactory::CreateGameObject("data\\Player.json");
        std::shared_ptr<GameObject> Monster = GameObjectFactory::CreateGameObject("data\\Monster.json");

        do
        {
            // IMPORTANT: We need to let GLib do it's thing. 
            GLib::Service(g_bQuit);

            if (!g_bQuit)
            {
                //static GLib::Point2D	GoodOffset = playerStart.ToGlibPoint2D();
                //static GLib::Point2D	BadOffset = monsterStart.ToGlibPoint2D();

                // IMPORTANT: Tell GLib that we want to start rendering
                GLib::BeginRendering(DirectX::Colors::Blue);
                // Tell GLib that we want to render some sprites
                GLib::Sprites::BeginRendering();

                if (Player && Player->GetComponent("RenderComponent") != nullptr)
                {
                    static float			moveDist = .03f;
                    
                    //MovetoTarget(Player->Position, playerTarget, moveDist);

                    // Tell GLib to render this sprite at our calculated location
                    //GLib::Render(*pGoodGuy, GoodOffset, 0.0f, 0.0f);
                }
                if (Monster && Monster->GetComponent("RenderComponent") != nullptr)
                {
                    static float			moveDist = .005f;

                    Point2D velocity = Player->Position - Monster->Position;
                    Vector3 temp(velocity, 0.0f);
                    temp.Normalize();
                    velocity = Point2D{ temp.x, temp.y };
                    velocity *= 300.0f;
                    MovableSystem::SetVelocity(Monster, velocity);

                    //MovetoTarget(Monster->Position, Player->Position, moveDist);

                    // Tell GLib to render this sprite at our calculated location
                    //GLib::Render(*pBadGuy, BadOffset, 0.0f, 0.0f);
                }

                // update game state
                GameState::Update();

                // update g_bQuit
                g_bQuit = GameState::getQuit();

            }
        } while (g_bQuit == false);
        

        // release memory
        GameState::ReleaseMemory();
        int a = Player.use_count(); // a = 1
        Player = nullptr; // there shouldn't be any pointer to the game object
        Monster = nullptr;

        // IMPORTANT:  Tell GLib to shutdown, releasing resources.
        GLib::Shutdown();
    }

    FreeConsole();

#if defined _DEBUG
    _CrtDumpMemoryLeaks();
#endif // _DEBUG

}

void MovetoTarget(Point2D& start, Point2D target, float moveDist) {
    int r = rand() % 100;
    bool moveX = false;
    bool moveY = false;
    if (target.x != start.x) moveX = true;
    if (target.y != start.y) moveY = true;

    if (moveX && moveY) {
        if (r < 50) {
            if (target.x < start.x) {
                start.x -= moveDist;
            }
            else {
                start.x += moveDist;
            }
        }
        else {
            if (target.y < start.y) {
                start.y -= moveDist;
            }
            else {
                start.y += moveDist;
            }
        }
    }
    else if (moveX) {
        if (target.x < start.x) {
            start.x -= moveDist;
        }
        else {
            start.x += moveDist;
        }
    }
    else if (moveY) {
        if (target.y < start.y) {
            start.y -= moveDist;
        }
        else {
            start.y += moveDist;
        }
    }
}


/*

int main() 
{
    Point2DUnitTest();
    printf("Unit Test Passed!\n\n");
    RandomNum rnd = RandomNum();
    char* playerName;
    std::cout << "Welcome to MonsterChase!\n\nHow many monsters to start:";
    int mstNum;
    do {
        if (std::cin >> mstNum) break;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\nYou have to enter a number. Try again: ";
    } while (true);
    Linkedlist monsterList = Linkedlist();

    // generate player loc
    Point2D playerLoc(rnd.getRnd() % 63, rnd.getRnd() % 63);

    // enter monster name
    for (int cnt = 0; cnt < mstNum; cnt++) {
        printf("\n\nEnter name for Monster %d : ", cnt);
        char *str = enterName();
        printf("The name of monster %d is : %s", cnt, str);
        Monster* monster = new Monster(str, rnd.getRnd()%63, rnd.getRnd()%63, cnt);
        // make sure monster is not generated at player's location
        if (monster->getLoc() == playerLoc) {
            if (monster->getLoc().x > 0) monster->getLoc() += Point2D(-1, 0);
            else monster->getLoc() += Point2D(1, 0);
            //else monster->getLoc().x++;
        }
        Node* node = new Node(monster);
        monsterList.insertNode(node);
    }

    // enter player name
    printf("\n\nEnter name for Player : ");
    playerName = enterName();
    printf("The name of Player is : %s", playerName);

    Player player(playerName, playerLoc);

    // enter game loop
    fseek(stdin, 0, SEEK_END);
    char keyboard = '!';
    char legalInputs[] = { possibleInputs };

    printMonster(monsterList); // print monster locations
    printf("\n\nPlayer:\n%s is at [%d, %d]", playerName, playerLoc.x , playerLoc.y); // print player location


    while (keyboard != 'q')
    {
        // get key
        printf("\n\nPress A to move left, D to move right, W to move up, S to move down, M to add a monster, N to delete the oldest monster, or Q to quit");
        printf("\nNotice: The map size is %d * %d", mapsize, mapsize);
        keyboard = _getch();

        // check whether input is legal
        bool illegal = true;
        for (char i : legalInputs) {
            if (keyboard == i) { illegal = false; break; }
        }
        if (illegal) {
            printf("\n\nYour input %c is illegal. Try again.", keyboard);
            continue;
        }

        if (keyboard == 'a' && playerLoc.x  > 0) {
            playerLoc.x --;
        }
        else if (keyboard == 'd' && playerLoc.x  < mapsize) {
            playerLoc.x ++;
        }
        else if (keyboard == 'w' && playerLoc.y > 0) {
            playerLoc.y--;
        }
        else if (keyboard == 's' && playerLoc.y < mapsize) {
            playerLoc.y++;
        }
        else if (keyboard == 'm') {
            printf("\n\nYou created a new monster. Enter the name of it: ");
            char* str = enterName();
            printf("The name of the new monster is : %s", str);
            Monster* monster = new Monster(str, rnd.getRnd() % 63, rnd.getRnd() % 63, monsterList.getLength());
            // make sure monster is not generated at player's location
            if (monster->getLoc() == playerLoc) {
                if (monster->getLoc().x > 0) monster->getLoc() += Point2D(-1, 0);
                else monster->getLoc() += Point2D(1, 0);
            }
            Node* node = new Node(monster);
            monsterList.insertNode(node);
        }
        else if (keyboard == 'n') {
            printf("\n\nYou destroyed the oldest monster %s", monsterList.getNode(0)->value -> getName());
            monsterList.deleteNode(0);
        }
        else if (keyboard == 'q') continue;

        printMonster(monsterList); // print monster locations
        printf("\n\nPlayer:\n%s is at [%d, %d]", playerName, playerLoc.x , playerLoc.y); // print player location


        Node* temp = monsterList.head;

        // Check for empty list.
        if (temp == nullptr) {
            printf("List empty");
            continue;
        }

        bool caught = false;
        // Traverse the list.
        while (temp != nullptr) {
            if(Monster* m = dynamic_cast<Monster*>(temp -> value))
            caught = m -> chase(playerLoc.x , playerLoc.y);
            if (caught) {
                printf("You are caught!");
                rnd.releaseMem();
                return 0;
            }
            temp = temp->next;
        }

    }
    rnd.releaseMem();
    return 0;

}
*/
