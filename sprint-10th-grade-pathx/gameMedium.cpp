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
static bool coins[GRID_H][GRID_W] = { false }; // Coin array
static int coinsCollected = 0;
static int totalCoins = 0;

static int playerX = 0;
static int playerY = 0;

static int endX = GRID_W - 1;
static int endY = GRID_H - 1;

static bool gameStarted = false;
static bool reachedEnd = false;
static float startTime = 0.0f;
static float elapsedTime = 0.0f;

static double blinkTimer = 0.0;
static bool blinkLight = false;

// Initialize coins in the maze
static void InitializeCoins() {
    coinsCollected = 0;
    totalCoins = 0;

    // Reset all coins
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            coins[y][x] = false;
        }
    }

    // Place coins randomly (about 15% of cells)
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            // Don't place coins at start or end positions
            if ((x == 0 && y == 0) || (x == endX && y == endY)) {
                continue;
            }

            // Random chance to place coin (15% probability)
            if (rand() % 100 < 15) {
                coins[y][x] = true;
                totalCoins++;
            }
        }
    }
}

static void GenerateMazeDFS(int x, int y) {
    maze[y][x].topWall = maze[y][x].topWall;
    maze[y][x].bottomWall = maze[y][x].bottomWall;
    maze[y][x].leftWall = maze[y][x].leftWall;
    maze[y][x].rightWall = maze[y][x].rightWall;

    bool visited[GRID_H][GRID_W] = { false };
    visited[y][x] = true;

    std::vector<Vector2> stack;
    stack.push_back({ (float)x, (float)y });

    int dirs[4] = { 0, 1, 2, 3 };

    while (!stack.empty()) {
        Vector2 cell = stack.back();
        int cx = (int)cell.x;
        int cy = (int)cell.y;

        std::vector<int> neighbors;

        if (cy > 0 && !visited[cy - 1][cx]) neighbors.push_back(0); // top
        if (cx < GRID_W - 1 && !visited[cy][cx + 1]) neighbors.push_back(1); // right
        if (cy < GRID_H - 1 && !visited[cy + 1][cx]) neighbors.push_back(2); // bottom
        if (cx > 0 && !visited[cy][cx - 1]) neighbors.push_back(3); // left

        if (!neighbors.empty()) {
            int dir = neighbors[rand() % neighbors.size()];

            if (dir == 0) { maze[cy][cx].topWall = false; maze[cy - 1][cx].bottomWall = false; cy--; }
            else if (dir == 1) { maze[cy][cx].rightWall = false; maze[cy][cx + 1].leftWall = false; cx++; }
            else if (dir == 2) { maze[cy][cx].bottomWall = false; maze[cy + 1][cx].topWall = false; cy++; }
            else if (dir == 3) { maze[cy][cx].leftWall = false; maze[cy][cx - 1].rightWall = false; cx--; }

            stack.push_back({ (float)cx, (float)cy });
            visited[cy][cx] = true;
        }
        else {
            stack.pop_back();
        }
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

            // Draw coin if it exists and hasn't been collected
            if (coins[y][x]) {
                DrawCircle(sx + cell / 2, sy + cell / 2, cell / 6, YELLOW);
            }
        }
    }

    DrawRectangle(ox + endX * cell + 2, oy + endY * cell + 2, cell - 4, cell - 4, GREEN);
}

// Separate function to draw coin counter
static void DrawCoinCounter() {
    // Background for coin counter
    DrawRectangle(440, 45, 250, 40, Fade(BLACK, 0.7f));

    // Coin icon
    DrawCircle(450, 65, 12, YELLOW);

    // Coin counter text
    DrawText("COINS:", 470, 50, 30, WHITE);
    DrawText(TextFormat("%d/%d", coinsCollected, totalCoins), 600, 50, 30, YELLOW);
}

void StartMediumGame() {
    srand((unsigned int)time(0));
    GenerateMazeDFS(0, 0);
    InitializeCoins(); // Initialize coins after generating maze

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

    playerX = 0;
    playerY = 0;
    gameStarted = false;
    reachedEnd = false;
    startTime = 0.0f;
    elapsedTime = 0.0f;

    while (!WindowShouldClose()) {

        blinkTimer += GetFrameTime();
        if (!blinkLight && blinkTimer >= 4.5) {
            blinkTimer = 0.0;
            blinkLight = true;
        }
        else if (blinkLight && blinkTimer >= 2.0) {
            blinkTimer = 0.0;
            blinkLight = false;
        }

        if (gameStarted) {
            elapsedTime = (float)GetTime() - startTime;
        }

        // Store old position for coin collection
        int oldX = playerX;
        int oldY = playerY;

        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && !maze[playerY][playerX].topWall) playerY--;
        if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && !maze[playerY][playerX].bottomWall) playerY++;
        if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && !maze[playerY][playerX].leftWall) playerX--;
        if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && !maze[playerY][playerX].rightWall) playerX++;

        // Collect coin if player moved to a new cell with a coin
        if ((playerX != oldX || playerY != oldY) && coins[playerY][playerX]) {
            coins[playerY][playerX] = false;
            coinsCollected++;
        }

        if (!gameStarted && (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) ||
            IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_RIGHT))) {
            gameStarted = true;
            startTime = (float)GetTime();
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (blinkLight) {
            DrawMazeLines(cell, offsetX, offsetY);
            DrawRectangle(offsetX + playerX * cell + 2, offsetY + playerY * cell + 2, cell - 4, cell - 4, YELLOW);

            int totalSeconds = (int)elapsedTime;
            int minutes = totalSeconds / 60;
            int seconds = totalSeconds % 60;

            // Draw time counter
            DrawText("TIME:", 440, 0, 40, WHITE);
            DrawText(TextFormat("%02d:%02d", minutes, seconds), 560, 0, 40, WHITE);

            // Draw coin counter ONLY when maze is visible
            DrawCoinCounter();
        }
        // Always show timer and coin counter, even when screen is black
        else {
            int totalSeconds = (int)elapsedTime;
            int minutes = totalSeconds / 60;
            int seconds = totalSeconds % 60;

            // Draw time counter
            DrawText("TIME:", 440, 0, 40, WHITE);
            DrawText(TextFormat("%02d:%02d", minutes, seconds), 560, 0, 40, WHITE);

            // Draw coin counter
            DrawCoinCounter();
        }

        EndDrawing();

        if (playerX == endX && playerY == endY) break;
    }

    CloseWindow();
}