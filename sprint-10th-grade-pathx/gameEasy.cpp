#include "raylib.h"
#include "gameEasy.h"

#define GRID_W 16
#define GRID_H 16

struct Cell { bool visited, topWall, bottomWall, leftWall, rightWall; };

static Cell maze[GRID_W][GRID_H];
static int px = 0, py = 0, endX = GRID_W - 1, endY = GRID_H - 1;
static int CELL_SIZE;

static void InitializeMaze() {
    for (int x = 0; x < GRID_W; x++)
        for (int y = 0; y < GRID_H; y++)
            maze[x][y] = { false,true,true,true,true };
}

static void GenerateMaze(int x, int y) {
    maze[x][y].visited = true;
    int dirs[4] = { 0,1,2,3 };
    for (int i = 0; i < 4; i++) {
        int r = GetRandomValue(i, 3);
        int t = dirs[i]; dirs[i] = dirs[r]; dirs[r] = t;
    }
    for (int i = 0; i < 4; i++) {
        int nx = x, ny = y;
        if (dirs[i] == 0) ny--;
        if (dirs[i] == 1) nx++;
        if (dirs[i] == 2) ny++;
        if (dirs[i] == 3) nx--;
        if (nx < 0 || nx >= GRID_W || ny < 0 || ny >= GRID_H) continue;
        if (maze[nx][ny].visited) continue;
        if (dirs[i] == 0) { maze[x][y].topWall = false; maze[nx][ny].bottomWall = false; }
        if (dirs[i] == 1) { maze[x][y].rightWall = false; maze[nx][ny].leftWall = false; }
        if (dirs[i] == 2) { maze[x][y].bottomWall = false; maze[nx][ny].topWall = false; }
        if (dirs[i] == 3) { maze[x][y].leftWall = false; maze[nx][ny].rightWall = false; }
        GenerateMaze(nx, ny);
    }
}

static void DrawMaze() {
    for (int x = 0; x < GRID_W; x++)
        for (int y = 0; y < GRID_H; y++) {
            int sx = x * CELL_SIZE, sy = y * CELL_SIZE;
            if (maze[x][y].topWall) DrawLine(sx, sy, sx + CELL_SIZE, sy, WHITE);
            if (maze[x][y].leftWall) DrawLine(sx, sy, sx, sy + CELL_SIZE, WHITE);
            if (maze[x][y].rightWall) DrawLine(sx + CELL_SIZE, sy, sx + CELL_SIZE, sy + CELL_SIZE, WHITE);
            if (maze[x][y].bottomWall) DrawLine(sx, sy + CELL_SIZE, sx + CELL_SIZE, sy + CELL_SIZE, WHITE);
        }
    DrawRectangle(endX * CELL_SIZE + 10, endY * CELL_SIZE + 10, CELL_SIZE - 20, CELL_SIZE - 20, RED);
}

static void MovePlayer() {
    if (IsKeyPressed(KEY_W) && !maze[px][py].topWall && py > 0) py--;
    if (IsKeyPressed(KEY_S) && !maze[px][py].bottomWall && py < GRID_H - 1) py++;
    if (IsKeyPressed(KEY_A) && !maze[px][py].leftWall && px > 0) px--;
    if (IsKeyPressed(KEY_D) && !maze[px][py].rightWall && px < GRID_W - 1) px++;
}

void StartEasyGame() {
    InitWindow(0, 0, "EASY MAZE"); ToggleFullscreen();
    CELL_SIZE = GetScreenWidth() / GRID_W;
    InitializeMaze(); GenerateMaze(0, 0);
    px = 0; py = 0; bool win = false;
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (!win) MovePlayer();
        if (px == endX && py == endY) win = true;

        BeginDrawing();
        ClearBackground(BLACK);
        DrawMaze();
        DrawCircle(px * CELL_SIZE + CELL_SIZE / 2, py * CELL_SIZE + CELL_SIZE / 2, CELL_SIZE / 3, YELLOW);
        if (win) {
            DrawText("YOU WIN!", GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 - 50, 60, GOLD);
            DrawText("Press ESC to exit", GetScreenWidth() / 2 - 140, GetScreenHeight() / 2 + 20, 30, RAYWHITE);
        }
        EndDrawing();
        if (IsKeyPressed(KEY_ESCAPE)) break;
    }
    CloseWindow();
}
