#include "gameSecMaze.h"
#include <vector>
#include <cstdlib>
#include <ctime>

// maze
static SecCell maze[SEC_GRID_H][SEC_GRID_W];
static SecPlayer player;

// grids end
static int endX = SEC_GRID_W - 1;
static int endY = SEC_GRID_H - 1;
static float startTime = 0.0f;
static float remainingTime = 0.0f;

static Sound moveSound;
static Sound winSound;

// creating maze
static void InitializeMaze() {
    for (int y = 0; y < SEC_GRID_H; y++)
        for (int x = 0; x < SEC_GRID_W; x++)
            maze[y][x] = SecCell{};
}

// generating maze
static void GenerateMazeDFS(int x, int y) {
    bool visited[SEC_GRID_H][SEC_GRID_W] = { false };
    visited[y][x] = true;

    std::vector<Vector2> stack;
    stack.push_back({ (float)x, (float)y });

    while (!stack.empty()) {
        Vector2 cell = stack.back();
        int cx = (int)cell.x;
        int cy = (int)cell.y;

        std::vector<int> neighbors;
        if (cy > 0 && !visited[cy - 1][cx]) neighbors.push_back(0);
        if (cx < SEC_GRID_W - 1 && !visited[cy][cx + 1]) neighbors.push_back(1);
        if (cy < SEC_GRID_H - 1 && !visited[cy + 1][cx]) neighbors.push_back(2);
        if (cx > 0 && !visited[cy][cx - 1]) neighbors.push_back(3);

        if (!neighbors.empty()) {
            int dir = neighbors[rand() % neighbors.size()];

            if (dir == 0) { maze[cy][cx].topWall = false; maze[cy - 1][cx].bottomWall = false; cy--; }
            else if (dir == 1) { maze[cy][cx].rightWall = false; maze[cy][cx + 1].leftWall = false; cx++; }
            else if (dir == 2) { maze[cy][cx].bottomWall = false; maze[cy + 1][cx].topWall = false; cy++; }
            else if (dir == 3) { maze[cy][cx].leftWall = false; maze[cy][cx - 1].rightWall = false; cx--; }

            stack.push_back({ (float)cx,(float)cy });
            visited[cy][cx] = true;
        }
        else {
            stack.pop_back();
        }
    }
}

// writing the grid lines
static void DrawCell(int x, int y, int cellSize, int ox, int oy) {
    int sx = ox + x * cellSize;
    int sy = oy + y * cellSize;

    if (maze[y][x].topWall)    DrawLine(sx, sy, sx + cellSize, sy, WHITE);
    if (maze[y][x].leftWall)   DrawLine(sx, sy, sx, sy + cellSize, WHITE);
    if (maze[y][x].rightWall)  DrawLine(sx + cellSize, sy, sx + cellSize, sy + cellSize, WHITE);
    if (maze[y][x].bottomWall) DrawLine(sx, sy + cellSize, sx + cellSize, sy + cellSize, WHITE);

    if (maze[y][x].hasDoor) {
        Color c = RED;
        if (maze[y][x].doorColor == 2) c = BLUE;
        if (maze[y][x].doorColor == 3) c = GREEN;
        DrawRectangle(sx + 2, sy + 2, cellSize - 4, cellSize - 4, c);
    }

    if (maze[y][x].item != SEC_NONE) {
        Color c = YELLOW;
        if (maze[y][x].item == SEC_KEY_RED)   c = RED;
        if (maze[y][x].item == SEC_KEY_BLUE)  c = BLUE;
        if (maze[y][x].item == SEC_KEY_GREEN) c = GREEN;
        DrawCircle(sx + cellSize / 2, sy + cellSize / 2, cellSize / 4, c);
    }
}

// drawing the maze
static void DrawMaze(int cellSize, int ox, int oy) {
    for (int y = 0; y < SEC_GRID_H; y++)
        for (int x = 0; x < SEC_GRID_W; x++) {
            int dx = abs(x - player.x);
            int dy = abs(y - player.y);
            if (dx + dy <= player.visionRadius)
                DrawCell(x, y, cellSize, ox, oy);
            else
                DrawRectangle(ox + x * cellSize, oy + y * cellSize, cellSize, cellSize, BLACK);
        }

    DrawRectangle(ox + endX * cellSize, oy + endY * cellSize, cellSize, cellSize, GREEN);
    DrawRectangle(ox + player.x * cellSize, oy + player.y * cellSize, cellSize, cellSize, YELLOW);
}

// starting the game
void StartGameSecMaze() {
    srand((unsigned int)time(0));

    InitializeMaze();
    GenerateMazeDFS(0, 0);

    const int screenWidth = 640;
    const int screenHeight = 640;
    InitWindow(screenWidth, screenHeight, "Escape Maze");
    moveSound = LoadSound("move.wav");
    SetSoundVolume(moveSound, 1.0f);
    winSound = LoadSound("win.wav");
    SetSoundVolume(winSound, 1.0f);

    SetTargetFPS(60);

    int cellSize = screenWidth / SEC_GRID_W;
    int offsetX = (screenWidth - cellSize * SEC_GRID_W) / 2;
    int offsetY = (screenHeight - cellSize * SEC_GRID_H) / 2;

    player = SecPlayer{};
    startTime = (float)GetTime();
    const float timeLimit = 40.0f;

    while (!WindowShouldClose()) {
        remainingTime = timeLimit - ((float)GetTime() - startTime);

        int oldX = player.x;
        int oldY = player.y;

        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && !maze[player.y][player.x].topWall) player.y--;
        if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && !maze[player.y][player.x].bottomWall) player.y++;
        if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && !maze[player.y][player.x].leftWall) player.x--;
        if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && !maze[player.y][player.x].rightWall) player.x++;

        if (player.x != oldX || player.y != oldY) {
            PlaySound(moveSound);
        }

        if (player.x == endX && player.y == endY) {
            PlaySound(winSound);
            break;
        }
        if (remainingTime <= 0) break;

        BeginDrawing();
        ClearBackground(BLACK);
        DrawMaze(cellSize, offsetX, offsetY);

        DrawText(TextFormat("Time: %.1f", remainingTime), 10, 10, 20, WHITE);
        EndDrawing();
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        if (player.x == endX && player.y == endY)
            DrawText("YOU ESCAPED!", 200, 200, 50, GOLD);
        else
            DrawText("TIME UP!", 200, 200, 50, RED);

        DrawText(TextFormat("Time: %.1f", remainingTime > 0 ? remainingTime : 0), 200, 300, 30, WHITE);
        DrawText("Press ESC to Exit", 200, 400, 20, WHITE);
        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE)) break;
    }

    UnloadSound(moveSound);
    UnloadSound(winSound);

    CloseWindow();
}