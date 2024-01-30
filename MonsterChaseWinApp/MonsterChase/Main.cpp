#include "Main.h"
#include "Monster.h"
#include "Player.h"
#include "LinkedList.h"
#include "RandomNum.h"
#include "Point2DUnitTest.h"

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

#include <DirectXColors.h>

#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

#include "GLib.h"



#define mapsize 64
#define possibleInputs 'a', \
            's', \
            'd', \
            'w', \
            'm', \
            'n'


static float  playerMoveDist = 10.0f;

static int windowWidth = 800;
static int windowHeight = 600;

static GLib::Point2D playerStart = { -180.0f, -100.0f };
static GLib::Point2D monsterStart = { 180.0f, -100.0f };

bool g_bQuit = false;

GLib::Point2D playerTarget = playerStart;


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


void* LoadFile(const char* i_pFilename, size_t& o_sizeFile);
GLib::Sprite* CreateSprite(const char* i_pFilename);

void MovetoTarget(GLib::Point2D& start, GLib::Point2D target, float moveDist);

void TestKeyCallback(unsigned int i_VKeyID, bool bWentDown)
{
#ifdef _DEBUG
    const size_t	lenBuffer = 65;
    char			Buffer[lenBuffer];

    sprintf_s(Buffer, lenBuffer, "VKey 0x%04x went %s\n", i_VKeyID, bWentDown ? "down" : "up");
    OutputDebugStringA(Buffer);
#endif // __DEBUG


    // Check if the 'q' key was pressed
    if (i_VKeyID == 'Q' && bWentDown)
    {
        g_bQuit = true; 
    }

    if (i_VKeyID == 'W' && bWentDown)
    {
        playerTarget.y = min(windowHeight/2, playerTarget.y + playerMoveDist);
    }

    if (i_VKeyID == 'A' && bWentDown)
    {
        playerTarget.x = max(-windowWidth/2, playerTarget.x - playerMoveDist);
    }

    if (i_VKeyID == 'S' && bWentDown)
    {
        playerTarget.y = max(-windowHeight/2, playerTarget.y - playerMoveDist);
    }

    if (i_VKeyID == 'D' && bWentDown)
    {
        playerTarget.x = min(windowWidth/2, playerTarget.x + playerMoveDist);
    }
}

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
    // random seed
    srand(time(0));

    // IMPORTANT: first we need to initialize GLib
    bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "GLibTest", -1, windowWidth, windowHeight, true);

    if (bSuccess)
    {
        // IMPORTANT (if we want keypress info from GLib): Set a callback for notification of key presses
        GLib::SetKeyStateChangeCallback(TestKeyCallback);
       

        // Create a couple of sprites using our own helper routine CreateSprite
        GLib::Sprite* pGoodGuy = CreateSprite("data\\GoodGuy.dds");
        GLib::Sprite* pBadGuy = CreateSprite("data\\BadGuy.dds");

        do
        {
            // IMPORTANT: We need to let GLib do it's thing. 
            GLib::Service(g_bQuit);

            if (!g_bQuit)
            {
                static GLib::Point2D	GoodOffset = playerStart;
                static GLib::Point2D	BadOffset = monsterStart;

                // IMPORTANT: Tell GLib that we want to start rendering
                GLib::BeginRendering(DirectX::Colors::Blue);
                // Tell GLib that we want to render some sprites
                GLib::Sprites::BeginRendering();

                if (pGoodGuy)
                {
                    static float			moveDist = .03f;
                    
                    MovetoTarget(GoodOffset, playerTarget, moveDist);

                    // Tell GLib to render this sprite at our calculated location
                    GLib::Render(*pGoodGuy, GoodOffset, 0.0f, 0.0f);
                }
                if (pBadGuy)
                {
                    static float			moveDist = .02f;

                    MovetoTarget(BadOffset, GoodOffset, moveDist);

                    // Tell GLib to render this sprite at our calculated location
                    GLib::Render(*pBadGuy, BadOffset, 0.0f, 0.0f);
                }

                // Tell GLib we're done rendering sprites
                GLib::Sprites::EndRendering();
                // IMPORTANT: Tell GLib we're done rendering
                GLib::EndRendering();
            }
        } while (g_bQuit == false);

        if (pGoodGuy)
            GLib::Release(pGoodGuy);
        if (pBadGuy)
            GLib::Release(pBadGuy);

        // IMPORTANT:  Tell GLib to shutdown, releasing resources.
        GLib::Shutdown();
    }

#if defined _DEBUG
    _CrtDumpMemoryLeaks();
#endif // _DEBUG

}

GLib::Sprite* CreateSprite(const char* i_pFilename)
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

void* LoadFile(const char* i_pFilename, size_t& o_sizeFile)
{
    assert(i_pFilename != NULL);

    FILE* pFile = NULL;

    errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
    if (fopenError != 0)
        return NULL;

    assert(pFile != NULL);

    int FileIOError = fseek(pFile, 0, SEEK_END);
    assert(FileIOError == 0);

    long FileSize = ftell(pFile);
    assert(FileSize >= 0);

    FileIOError = fseek(pFile, 0, SEEK_SET);
    assert(FileIOError == 0);

    uint8_t* pBuffer = new uint8_t[FileSize];
    assert(pBuffer);

    size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
    assert(FileRead == FileSize);

    fclose(pFile);

    o_sizeFile = FileSize;

    return pBuffer;
}

void MovetoTarget(GLib::Point2D& start, GLib::Point2D target, float moveDist) {
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
