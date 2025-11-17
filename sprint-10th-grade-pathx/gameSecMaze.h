#ifndef GAME_SEC_MAZE_H
#define GAME_SEC_MAZE_H

#include "raylib.h"


int GetSecMazeCoins();
static const int SEC_GRID_W = 20;
static const int SEC_GRID_H = 20;


enum SecItemType {
    SEC_NONE = 0,
    SEC_KEY_RED,
    SEC_KEY_BLUE,
    SEC_KEY_GREEN,
    SEC_UPGRADE_LIGHT
};

struct SecCell {
    bool topWall = true;
    bool bottomWall = true;
    bool leftWall = true;
    bool rightWall = true;

    bool hasDoor = false;
    int doorColor = 0; // 1=RED, 2=BLUE, 3=GREEN

    SecItemType item = SEC_NONE;
};
struct SecPlayer {
    int x = 0;
    int y = 0;

    bool keys[3] = { false, false, false }; // red blue green
    int visionRadius = 3;
};
void StartGameSecMaze();

#endif
