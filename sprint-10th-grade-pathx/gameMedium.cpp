#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>

static const int GRID_W = 25;
static const int GRID_H = 25;

struct Cell {
    bool topWall = true;
    bool bottomWall = true;
    bool leftWall = true;
    bool rightWall = true;
};

static Cell maze[GRID_H][GRID_W];
static bool coins[GRID_H][GRID_W] = { false };
static int coinsCollected = 0;
static int totalCoins = 0;

static int playerX = 0;
static int playerY = 0;
static int endX = GRID_W - 1;
static int endY = GRID_H - 1;

static bool gameStarted = false;
static float startTime = 0.0f;
static float elapsedTime = 0.0f;

// Sounds
static Sound coinSound;
static Sound winSound;

// Blinking background
static float blinkTimer = 0.0f;
static bool blinkState = false; // false = dark, true = light

static void InitializeCoins() {
    coinsCollected = 0;
    totalCoins = 0;

    for (int y = 0; y < GRID_H; y++)
        for (int x = 0; x < GRID_W; x++)
            coins[y][x] = false;

    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            if ((x == 0 && y == 0) || (x == endX && y == endY)) continue;
            if (rand() % 100 < 15) {
                coins[y][x] = true;
                totalCoins++;
            }
        }
    }
}

static void GenerateMazeDFS(int x, int y) {
    bool visited[GRID_H][GRID_W] = { false };
    std::vector<Vector2> stack;

    stack.push_back({ (float)x, (float)y });
    visited[y][x] = true;

    while (!stack.empty()) {
        Vector2 cell = stack.back();
        int cx = (int)cell.x;
        int cy = (int)cell.y;

        std::vector<int> neighbors;
        if (cy > 0 && !visited[cy - 1][cx]) neighbors.push_back(0);
        if (cx < GRID_W - 1 && !visited[cy][cx + 1]) neighbors.push_back(1);
        if (cy < GRID_H - 1 && !visited[cy + 1][cx]) neighbors.push_back(2);
        if (cx > 0 && !visited[cy][cx - 1]) neighbors.push_back(3);

        if (!neighbors.empty()) {
            int dir = neighbors[rand() % neighbors.size()];
            if (dir == 0) { maze[cy][cx].topWall = false; maze[cy - 1][cx].bottomWall = false; cy--; }
            else if (dir == 1) { maze[cy][cx].rightWall = false; maze[cy][cx + 1].leftWall = false; cx++; }
            else if (dir == 2) { maze[cy][cx].bottomWall = false; maze[cy + 1][cx].topWall = false; cy++; }
            else if (dir == 3) { maze[cy][cx].leftWall = false; maze[cy][cx - 1].rightWall = false; cx--; }

            stack.push_back({ (float)cx, (float)cy });
            visited[cy][cx] = true;
        }
        else stack.pop_back();
    }
}

static void DrawMazeLines(int cell, int ox, int oy) {
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            int sx = ox + x * cell;
            int sy = oy + y * cell;

            if (maze[y][x].topWall) DrawLine(sx, sy, sx + cell, sy, WHITE);
            if (maze[y][x].leftWall) DrawLine(sx, sy, sx, sy + cell, WHITE);
            if (maze[y][x].rightWall) DrawLine(sx + cell, sy, sx + cell, sy + cell, WHITE);
            if (maze[y][x].bottomWall) DrawLine(sx, sy + cell, sx + cell, sy + cell, WHITE);

            if (coins[y][x])
                DrawCircle(sx + cell / 2, sy + cell / 2, cell / 6, YELLOW);
        }
    }

    DrawRectangle(ox + endX * cell + 2, oy + endY * cell + 2, cell - 4, cell - 4, GREEN);
}

static void DrawCoinCounter(int hudX, int hudY) {
    DrawRectangle(hudX, hudY + 70, 220, 40, Fade(BLACK, 0.7f));
    DrawCircle(hudX + 15, hudY + 90, 10, YELLOW);
    DrawText("COINS:", hudX + 35, hudY + 75, 25, WHITE);
    DrawText(TextFormat("%d/%d", coinsCollected, totalCoins), hudX + 120, hudY + 75, 25, YELLOW);
}

void StartMediumGame() {
    srand((unsigned int)time(0));

    // Load sounds
    coinSound = LoadSound("coins.wav");
    SetSoundVolume(coinSound, 1.0f);
    winSound = LoadSound("win.wav");
    SetSoundVolume(winSound, 1.0f);

    GenerateMazeDFS(0, 0);
    InitializeCoins();

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    SetTargetFPS(60);

    int cell = (screenWidth / GRID_W < screenHeight / GRID_H) ? screenWidth / GRID_W : screenHeight / GRID_H;
    int offsetX = (screenWidth - cell * GRID_W) / 2;
    int offsetY = (screenHeight - cell * GRID_H) / 2;

    playerX = 0;
    playerY = 0;
    gameStarted = false;
    startTime = 0.0f;
    elapsedTime = 0.0f;

    blinkTimer = 0.0f;
    blinkState = false;

    enum GameState { PLAYING, WINSCREEN };
    GameState state = PLAYING;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;

        // Update elapsed time
        if (gameStarted)
            elapsedTime = (float)GetTime() - startTime;

        // Update blinking timer
        blinkTimer += GetFrameTime();
        if (!blinkState && blinkTimer >= 4.0f) {
            blinkState = true;
            blinkTimer = 0.0f;
        }
        else if (blinkState && blinkTimer >= 2.5f) {

            blinkState = false;
            blinkTimer = 0.0f;
        }

        if (state == PLAYING) {
            int oldX = playerX, oldY = playerY;
            if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && !maze[playerY][playerX].topWall) playerY--;
            if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && !maze[playerY][playerX].bottomWall) playerY++;
            if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && !maze[playerY][playerX].leftWall) playerX--;
            if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && !maze[playerY][playerX].rightWall) playerX++;

            // Collect coin + play sound
            if ((playerX != oldX || playerY != oldY) && coins[playerY][playerX]) {
                coins[playerY][playerX] = false;
                coinsCollected++;
                PlaySound(coinSound);
            }

            if (!gameStarted &&
                (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) ||
                    IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_RIGHT))) {
                gameStarted = true;
                startTime = (float)GetTime();
            }

            BeginDrawing();
            if (blinkState) ClearBackground(LIGHTGRAY);
            else ClearBackground(BLACK);

            DrawMazeLines(cell, offsetX, offsetY);
            DrawRectangle(offsetX + playerX * cell + 2, offsetY + playerY * cell + 2, cell - 4, cell - 4, YELLOW);

            DrawText("TIME:", 10, 10, 30, WHITE);
            int minutes = (int)elapsedTime / 60;
            int seconds = (int)elapsedTime % 60;
            DrawText(TextFormat("%02d:%02d", minutes, seconds), 10, 40, 30, WHITE);

            DrawCoinCounter(10, 10);
            EndDrawing();

            // Check win
            if (playerX == endX && playerY == endY) {
                state = WINSCREEN;
                PlaySound(winSound);
            }
        }
        else if (state == WINSCREEN) {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("YOU WIN!", screenWidth / 2 - 150, screenHeight / 2 - 120, 60, GOLD);
            int minutes = (int)elapsedTime / 60;
            int seconds = (int)elapsedTime % 60;
            DrawText(TextFormat("Time: %02d:%02d", minutes, seconds), screenWidth / 2 - 100, screenHeight / 2, 40, WHITE);
            DrawText(TextFormat("Coins: %d/%d", coinsCollected, totalCoins), screenWidth / 2 - 100, screenHeight / 2 + 50, 40, YELLOW);
            DrawText("Press R to Play Again", screenWidth / 2 - 160, screenHeight / 2 + 120, 30, RAYWHITE);
            DrawText("Press ESC to Return to Menu", screenWidth / 2 - 200, screenHeight / 2 + 160, 30, RAYWHITE);
            EndDrawing();

            if (IsKeyPressed(KEY_R)) {
                StartMediumGame();
                return;
            }
        }
    }

    // Unload sounds
    UnloadSound(coinSound);
    UnloadSound(winSound);
}
