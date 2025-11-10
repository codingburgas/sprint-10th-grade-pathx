#include "raylib.h"
#include "game.h"
#include <functional>

#define SCREEN_WIDTH 2200
#define SCREEN_HEIGHT 1200
#define CELL_SIZE 100
#define GRID_W (SCREEN_WIDTH / CELL_SIZE)
#define GRID_H (SCREEN_HEIGHT / CELL_SIZE)

struct Cell {
    bool visited;
    bool topWall, bottomWall, leftWall, rightWall;
};

static Cell maze[GRID_W][GRID_H];
static int px = 0, py = 0;

// ------------------------------------------------------------

static void InitializeMaze() {
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            maze[x][y].visited = false;
            maze[x][y].topWall = true;
            maze[x][y].bottomWall = true;
            maze[x][y].leftWall = true;
            maze[x][y].rightWall = true;
        }
    }
}

static void GenerateMaze(int x, int y) {
    maze[x][y].visited = true;

    int dirs[4] = { 0,1,2,3 };

    for (int i = 0; i < 4; i++) {
        int r = GetRandomValue(i, 3);
        int t = dirs[i];
        dirs[i] = dirs[r];
        dirs[r] = t;
    }

    for (int i = 0; i < 4; i++) {
        int nx = x, ny = y;

        if (dirs[i] == 0) ny--;
        if (dirs[i] == 1) nx++;
        if (dirs[i] == 2) ny++;
        if (dirs[i] == 3) nx--;

        if (nx < 0 || nx >= GRID_W || ny < 0 || ny >= GRID_H) continue;
        if (maze[nx][ny].visited) continue;

        if (dirs[i] == 0) {
            maze[x][y].topWall = false;
            maze[nx][ny].bottomWall = false;
        }
        if (dirs[i] == 1) {
            maze[x][y].rightWall = false;
            maze[nx][ny].leftWall = false;
        }
        if (dirs[i] == 2) {
            maze[x][y].bottomWall = false;
            maze[nx][ny].topWall = false;
        }
        if (dirs[i] == 3) {
            maze[x][y].leftWall = false;
            maze[nx][ny].rightWall = false;
        }

        GenerateMaze(nx, ny);
    }
}

static void DrawMaze() {
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            int sx = x * CELL_SIZE;
            int sy = y * CELL_SIZE;

            if (maze[x][y].topWall) DrawLine(sx, sy, sx + CELL_SIZE, sy, WHITE);
            if (maze[x][y].leftWall) DrawLine(sx, sy, sx, sy + CELL_SIZE, WHITE);
            if (maze[x][y].rightWall) DrawLine(sx + CELL_SIZE, sy, sx + CELL_SIZE, sy + CELL_SIZE, WHITE);
            if (maze[x][y].bottomWall) DrawLine(sx, sy + CELL_SIZE, sx + CELL_SIZE, sy + CELL_SIZE, WHITE);
        }
    }
}

static void MovePlayer() {
    if (IsKeyPressed(KEY_W) && !maze[px][py].topWall) py--;
    if (IsKeyPressed(KEY_S) && !maze[px][py].bottomWall) py++;
    if (IsKeyPressed(KEY_A) && !maze[px][py].leftWall) px--;
    if (IsKeyPressed(KEY_D) && !maze[px][py].rightWall) px++;
}

// ------------------------------------------------------------
// PUBLIC FUNCTION CALLED FROM main.cpp
// ------------------------------------------------------------
void StartNewGame() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Game");
    SetTargetFPS(60);

    InitializeMaze();
    GenerateMaze(0, 0);

    px = 0;
    py = 0;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            CloseWindow();
            return;
        }

        MovePlayer();

        BeginDrawing();
        ClearBackground(BLACK);

        DrawMaze();
        DrawCircle(px * CELL_SIZE + CELL_SIZE / 2,
            py * CELL_SIZE + CELL_SIZE / 2,
            CELL_SIZE / 3,
            YELLOW);

        EndDrawing();
    }

    CloseWindow();
}
