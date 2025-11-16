#ifndef GAME_DOOR_MAZE_H
#define GAME_DOOR_MAZE_H

#include "raylib.h"

const int GRID_W = 20;
const int GRID_H = 20;

enum ItemType {
    NONE = 0,
    KEY_RED,
    KEY_BLUE,
    KEY_GREEN,
    UPGRADE_LIGHT
};

struct Cell {
    bool topWall = true;
    bool bottomWall = true;
    bool leftWall = true;
    bool rightWall = true;

    bool hasDoor = false;
    int doorColor = 0;

    ItemType item = NONE;
};

struct Player {
    int x = 0;
    int y = 0;

    bool keys[3] = { false, false, false };
    int visionRadius = 3;
};

void StartGameDoorMaze();

#endif
