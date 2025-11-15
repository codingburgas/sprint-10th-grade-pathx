#pragma once
#include "raylib.h"

static const int GRID_W = 16;
static const int GRID_H = 16;

enum ItemType { NONE, KEY_RED, KEY_BLUE, KEY_GREEN, UPGRADE_LIGHT };

struct Cell {
    bool topWall = true;
    bool bottomWall = true;
    bool leftWall = true;
    bool rightWall = true;

    bool hasDoor = false;
    int doorColor = 0; // 1=red,2=blue,3=green

    ItemType item = NONE; // key or upgrade
};

struct Player {
    int x = 0;
    int y = 0;
    bool keys[3] = { false, false, false }; // red, blue, green
    int visionRadius = 3;
};

void StartGameDoorMaze();
