#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>

static const int GRID_W = 25;
static const int GRID_H = 25;

// Maze cell with 4 walls
struct Cell {
    bool topWall = true;
    bool bottomWall = true;
    bool leftWall = true;
    bool rightWall = true;
};

static Cell maze[GRID_H][GRID_W];

// Coin data
static bool coins[GRID_H][GRID_W] = { false };
static int coinsCollected = 0;
static int totalCoins = 0;

// Player position
static int playerX = 0;
static int playerY = 0;

// Finish point
static int endX = GRID_W - 1;
static int endY = GRID_H - 1;

// Timer
static bool gameStarted = false;
static float startTime = 0.0f;
static float elapsedTime = 0.0f;

// Light blinking
static double blinkTimer = 0.0;
static bool blinkLight = false;

// ------------------ COINS ------------------

// Put coins randomly in maze
static void InitializeCoins() {
    coinsCollected = 0;
    totalCoins = 0;

    // Remove all coins first
    for (int y = 0; y < GRID_H; y++)
        for (int x = 0; x < GRID_W; x++)
            coins[y][x] = false;

    // Randomly place new coins
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {

            // No coin at start or end
            if ((x == 0 && y == 0) || (x == endX && y == endY)) continue;

            // 15% chance to create a coin
            if (rand() % 100 < 15) {
                coins[y][x] = true;
                totalCoins++;
            }
        }
    }
}

// ------------------ MAZE GENERATION ------------------

// Create maze using DFS algorithm
static void GenerateMazeDFS(int x, int y) {
    bool visited[GRID_H][GRID_W] = { false };
    std::vector<Vector2> stack;

    stack.push_back({ (float)x, (float)y });
    visited[y][x] = true;

    while (!stack.empty()) {

        Vector2 cell = stack.back();
        int cx = (int)cell.x;
        int cy = (int)cell.y;

        // Check available neighbors
        std::vector<int> neighbors;
        if (cy > 0 && !visited[cy - 1][cx]) neighbors.push_back(0);
        if (cx < GRID_W - 1 && !visited[cy][cx + 1]) neighbors.push_back(1);
        if (cy < GRID_H - 1 && !visited[cy + 1][cx]) neighbors.push_back(2);
        if (cx > 0 && !visited[cy][cx - 1]) neighbors.push_back(3);

        // If there are unvisited neighbors
        if (!neighbors.empty()) {
            int dir = neighbors[rand() % neighbors.size()];

            // Remove walls
            if (dir == 0) { maze[cy][cx].topWall = false; maze[cy - 1][cx].bottomWall = false; cy--; }
            else if (dir == 1) { maze[cy][cx].rightWall = false; maze[cy][cx + 1].leftWall = false; cx++; }
            else if (dir == 2) { maze[cy][cx].bottomWall = false; maze[cy + 1][cx].topWall = false; cy++; }
            else if (dir == 3) { maze[cy][cx].leftWall = false; maze[cy][cx - 1].rightWall = false; cx--; }

            // Move to next cell
            stack.push_back({ (float)cx, (float)cy });
            visited[cy][cx] = true;
        }
        else {
            // Go back
            stack.pop_back();
        }
    }
}

// ------------------ DRAW MAZE ------------------

// Draw walls + coins
static void DrawMazeLines(int cell, int ox, int oy) {
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {

            int sx = ox + x * cell;
            int sy = oy + y * cell;

            // Draw walls
            if (maze[y][x].topWall) DrawLine(sx, sy, sx + cell, sy, WHITE);
            if (maze[y][x].leftWall) DrawLine(sx, sy, sx, sy + cell, WHITE);
            if (maze[y][x].rightWall) DrawLine(sx + cell, sy, sx + cell, sy + cell, WHITE);
            if (maze[y][x].bottomWall) DrawLine(sx, sy + cell, sx + cell, sy + cell, WHITE);

            // Draw coin
            if (coins[y][x])
                DrawCircle(sx + cell / 2, sy + cell / 2, cell / 6, YELLOW);
        }
    }

    // Draw exit cell
    DrawRectangle(ox + endX * cell + 2, oy + endY * cell + 2, cell - 4, cell - 4, GREEN);
}

// ------------------ COIN COUNTER DISPLAY ------------------

static void DrawCoinCounter() {
    DrawRectangle(440, 45, 250, 40, Fade(BLACK, 0.7f));
    DrawCircle(450, 65, 12, YELLOW);
    DrawText("COINS:", 470, 50, 30, WHITE);
    DrawText(TextFormat("%d/%d", coinsCollected, totalCoins), 600, 50, 30, YELLOW);
}

// ------------------ MAIN MEDIUM GAME ------------------

void StartMediumGame() {

    srand((unsigned int)time(0));

    // Build maze and coins
    GenerateMazeDFS(0, 0);
    InitializeCoins();

    const int screenWidth = 720;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Maze - Medium");
    SetTargetFPS(60);

    // Cell and offset to center maze
    int cell = (screenWidth / GRID_W < screenHeight / GRID_H) ? screenWidth / GRID_W : screenHeight / GRID_H;
    int offsetX = (screenWidth - cell * GRID_W) / 2;
    int offsetY = (screenHeight - cell * GRID_H) / 2;

    // Reset player and timer
    playerX = 0;
    playerY = 0;
    gameStarted = false;
    startTime = 0.0f;
    elapsedTime = 0.0f;

    enum GameState { PLAYING, WINSCREEN };
    GameState state = PLAYING;

    while (!WindowShouldClose()) {

        // Update blinking light timer
        blinkTimer += GetFrameTime();
        if (!blinkLight && blinkTimer >= 4.5) { blinkTimer = 0; blinkLight = true; }
        else if (blinkLight && blinkTimer >= 123.0) { blinkTimer = 0; blinkLight = false; }

        // Update timer
        if (gameStarted)
            elapsedTime = (float)GetTime() - startTime;

        // ---------------- PLAYING ----------------
        if (state == PLAYING) {

            int oldX = playerX, oldY = playerY;

            // Movement checks
            if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && !maze[playerY][playerX].topWall) playerY--;
            if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && !maze[playerY][playerX].bottomWall) playerY++;
            if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && !maze[playerY][playerX].leftWall) playerX--;
            if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && !maze[playerY][playerX].rightWall) playerX++;

            // Pick up coin
            if ((playerX != oldX || playerY != oldY) && coins[playerY][playerX]) {
                coins[playerY][playerX] = false;
                coinsCollected++;
            }

            // Start timer on first move
            if (!gameStarted &&
                (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) ||
                    IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_RIGHT))) {

                gameStarted = true;
                startTime = (float)GetTime();
            }

            // Draw screen
            BeginDrawing();
            ClearBackground(BLACK);

            // Show maze only when blinkLight is ON
            if (blinkLight) {
                DrawMazeLines(cell, offsetX, offsetY);
                DrawRectangle(offsetX + playerX * cell + 2, offsetY + playerY * cell + 2,
                    cell - 4, cell - 4, YELLOW);
            }

            // Draw timer
            int totalSeconds = (int)elapsedTime;
            int minutes = totalSeconds / 60;
            int seconds = totalSeconds % 60;

            DrawText("TIME:", 440, 0, 40, WHITE);
            DrawText(TextFormat("%02d:%02d", minutes, seconds), 560, 0, 40, WHITE);

            // Draw coins HUD
            DrawCoinCounter();

            EndDrawing();

            // Check win
            if (playerX == endX && playerY == endY)
                state = WINSCREEN;
        }

        // ---------------- WIN SCREEN ----------------
        else if (state == WINSCREEN) {

            BeginDrawing();
            ClearBackground(BLACK);

            DrawText("YOU WIN!", screenWidth / 2 - 150, screenHeight / 2 - 120, 60, GOLD);

            int totalSeconds = (int)elapsedTime;
            int minutes = totalSeconds / 60;
            int seconds = totalSeconds % 60;

            DrawText(TextFormat("Time: %02d:%02d", minutes, seconds), screenWidth / 2 - 100, screenHeight / 2, 40, WHITE);
            DrawText(TextFormat("Coins: %d/%d", coinsCollected, totalCoins), screenWidth / 2 - 100, screenHeight / 2 + 50, 40, YELLOW);

            DrawText("Press R to Play Again", screenWidth / 2 - 160, screenHeight / 2 + 120, 30, RAYWHITE);
            DrawText("Press ESC for Menu", screenWidth / 2 - 140, screenHeight / 2 + 160, 30, RAYWHITE);

            EndDrawing();

            // Restart
            if (IsKeyPressed(KEY_R)) {
                CloseWindow();
                StartMediumGame();
                return;
            }

            // Exit to menu
            if (IsKeyPressed(KEY_ESCAPE)) break;
        }
    }

    CloseWindow();
}
