#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include "gameMedium.h"

static const int GRID_W = 25;
static const int GRID_H = 25;

static int maze[GRID_H][GRID_W];

static int playerX = 0;
static int playerY = 0;

static int endX = GRID_W - 1;
static int endY = GRID_H - 1;

static double blinkTimer = 0.0;
static bool blinkLight = false;

// Добавяне на допълнителни случайни пътища
static void AddExtraPaths(int count) {
    for (int i = 0; i < count; i++) {
        int x = rand() % GRID_W;
        int y = rand() % GRID_H;

        int dirs[4] = { 0,1,2,3 };
        int r = dirs[rand() % 4];

        int nx = x, ny = y;
        if (r == 0) ny--;
        if (r == 1) nx++;
        if (r == 2) ny++;
        if (r == 3) nx--;

        if (nx < 0 || nx >= GRID_W || ny < 0 || ny >= GRID_H) continue;
        maze[y][x] = 0;
        maze[ny][nx] = 0;
    }
}

static void GenerateMaze() {
    for (int y = 0; y < GRID_H; y++)
        for (int x = 0; x < GRID_W; x++)
            maze[y][x] = 1;

    playerX = 0; playerY = 0;
    maze[0][0] = 0;

    std::vector<Vector2> stack;
    stack.push_back({ 0,0 });

    while (!stack.empty()) {
        Vector2 cell = stack.back();
        int cx = (int)cell.x;
        int cy = (int)cell.y;

        std::vector<Vector2> neighbors;

        if (cy - 2 > 0 && maze[cy - 2][cx] == 1) neighbors.push_back({ (float)cx,(float)(cy - 2) });
        if (cy + 2 < GRID_H && maze[cy + 2][cx] == 1) neighbors.push_back({ (float)cx,(float)(cy + 2) });
        if (cx - 2 > 0 && maze[cy][cx - 2] == 1) neighbors.push_back({ (float)(cx - 2),(float)cy });
        if (cx + 2 < GRID_W && maze[cy][cx + 2] == 1) neighbors.push_back({ (float)(cx + 2),(float)cy });

        if (!neighbors.empty()) {
            Vector2 next = neighbors[rand() % neighbors.size()];
            int nx = (int)next.x;
            int ny = (int)next.y;
            maze[(ny + cy) / 2][(nx + cx) / 2] = 0;
            maze[ny][nx] = 0;
            stack.push_back({ (float)nx,(float)ny });
        }
        else {
            stack.pop_back();
        }
    }

    AddExtraPaths((GRID_W * GRID_H) / 3); // повече объркващи пътища
}

static void DrawMaze(int cell, int ox, int oy) {
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            int px = ox + x * cell;
            int py = oy + y * cell;

            if (maze[y][x] == 1) {
                DrawRectangle(px, py, cell, cell, DARKGRAY);
                DrawRectangleLinesEx(Rectangle{ (float)px,(float)py,(float)cell,(float)cell }, 1, RAYWHITE);
            }
        }
    }

    DrawRectangle(ox + endX * cell, oy + endY * cell, cell, cell, GREEN);
}

void StartMediumGame() {
    srand((unsigned int)time(0)); // предупреждение C4244 избегнато
    GenerateMaze();

    const int screenWidth = 720;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Maze - Medium");
    SetTargetFPS(60);

    int cell = (screenWidth / GRID_W < screenHeight / GRID_H)
        ? screenWidth / GRID_W
        : screenHeight / GRID_H;
    if (cell < 4) cell = 4;

    int offsetX = (screenWidth - cell * GRID_W) / 2;
    int offsetY = (screenHeight - cell * GRID_H) / 2;

    while (!WindowShouldClose()) {
        blinkTimer += GetFrameTime();
        if (blinkTimer >= 0.5) {
            blinkTimer = 0;
            blinkLight = !blinkLight;
        }

        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
            if (playerY > 0 && maze[playerY - 1][playerX] == 0) playerY--;
        }
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
            if (playerY < GRID_H - 1 && maze[playerY + 1][playerX] == 0) playerY++;
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
            if (playerX > 0 && maze[playerY][playerX - 1] == 0) playerX--;
        }
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
            if (playerX < GRID_W - 1 && maze[playerY][playerX + 1] == 0) playerX++;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (blinkLight) {
            DrawMaze(cell, offsetX, offsetY);
            DrawRectangle(offsetX + playerX * cell, offsetY + playerY * cell, cell, cell, YELLOW);
        }

        EndDrawing();

        if (playerX == endX && playerY == endY) break;
    }

    CloseWindow();
}
