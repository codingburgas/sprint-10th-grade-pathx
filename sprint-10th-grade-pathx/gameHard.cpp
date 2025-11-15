#include "raylib.h"

#define GRID_W 16
#define GRID_H 16

// One cell of the maze (walls + visited flag)
struct Cell {
    bool visited, topWall, bottomWall, leftWall, rightWall;
};

static Cell maze[GRID_W][GRID_H];

// Coins placed in maze
static bool coins[GRID_W][GRID_H] = { false };
static int totalCoins = 0;
static int coinsCollected = 0;

// Player and finish position
static int px = 0, py = 0;
static int endX = GRID_W - 1, endY = GRID_H - 1;

// Timer settings
static bool gameStarted = false;
static float startTime = 0.0f;
static float elapsedTime = 0.0f;

// ---------------- COINS ----------------

// Create coins in random maze cells
static void InitializeCoins() {
    coinsCollected = 0; // Reset collected coins
    totalCoins = 0;

    // Remove all coins first
    for (int x = 0; x < GRID_W; x++)
        for (int y = 0; y < GRID_H; y++)
            coins[x][y] = false;

    // Place new coins randomly
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {

            // No coin at start or end
            if ((x == 0 && y == 0) || (x == endX && y == endY))
                continue;

            // 10% chance to place a coin
            if (GetRandomValue(0, 9) == 0) {
                coins[x][y] = true;
                totalCoins++;
            }
        }
    }
}


// ---------------- MAZE GENERATION ----------------

// Set all walls on and visited = false
static void InitializeMaze() {
    for (int x = 0; x < GRID_W; x++)
        for (int y = 0; y < GRID_H; y++)
            maze[x][y] = Cell{ false, true, true, true, true };
}

// Randomize direction order
static void ShuffleDirs(int d[4]) {
    for (int i = 0; i < 4; i++) {
        int r = GetRandomValue(i, 3);
        int t = d[i]; d[i] = d[r]; d[r] = t;
    }
}

// Recursive DFS maze generator
static void GenerateMaze(int x, int y) {
    maze[x][y].visited = true;

    int dirs[4] = { 0,1,2,3 };
    ShuffleDirs(dirs);

    for (int i = 0; i < 4; i++) {
        int nx = x, ny = y;

        // Move direction
        if (dirs[i] == 0) ny--;
        if (dirs[i] == 1) nx++;
        if (dirs[i] == 2) ny++;
        if (dirs[i] == 3) nx--;

        // Skip invalid or visited cells
        if (nx < 0 || nx >= GRID_W || ny < 0 || ny >= GRID_H) continue;
        if (maze[nx][ny].visited) continue;

        // Remove walls between two cells
        if (dirs[i] == 0) { maze[x][y].topWall = false;    maze[nx][ny].bottomWall = false; }
        if (dirs[i] == 1) { maze[x][y].rightWall = false;  maze[nx][ny].leftWall = false; }
        if (dirs[i] == 2) { maze[x][y].bottomWall = false; maze[nx][ny].topWall = false; }
        if (dirs[i] == 3) { maze[x][y].leftWall = false;   maze[nx][ny].rightWall = false; }

        GenerateMaze(nx, ny);
    }
}


// ---------------- DRAW MAZE ----------------

// Draw maze walls and coins
static void DrawMaze(int cellSize, int ox, int oy, bool drawCoins) {

    for (int x = 0; x < GRID_W; x++)
        for (int y = 0; y < GRID_H; y++) {

            int sx = ox + x * cellSize;
            int sy = oy + y * cellSize;

            // Draw walls
            if (maze[x][y].topWall)    DrawLine(sx, sy, sx + cellSize, sy, WHITE);
            if (maze[x][y].leftWall)   DrawLine(sx, sy, sx, sy + cellSize, WHITE);
            if (maze[x][y].rightWall)  DrawLine(sx + cellSize, sy, sx + cellSize, sy + cellSize, WHITE);
            if (maze[x][y].bottomWall) DrawLine(sx, sy + cellSize, sx + cellSize, sy + cellSize, WHITE);

            // Draw coin only if allowed
            if (drawCoins && coins[x][y])
                DrawCircle(sx + cellSize / 2, sy + cellSize / 2, cellSize / 6, YELLOW);
        }

    // Draw finish square
    DrawRectangle(ox + endX * cellSize + 5, oy + endY * cellSize + 5,
        cellSize - 10, cellSize - 10, RED);
}


// Draw coin count on screen
static void DrawCoinCounter() {

    // Dark box behind text
    DrawRectangle(510, 45, 200, 40, Fade(BLACK, 0.7f));

    // Coin icon
    DrawCircle(520, 65, 12, YELLOW);

    // Text
    DrawText("COINS:", 540, 50, 30, WHITE);
    DrawText(TextFormat("%d/%d", coinsCollected, totalCoins), 650, 50, 30, YELLOW);
}


// ---------------- PLAYER MOVEMENT ----------------

// Move player and collect coins
static void MovePlayer() {
    int oldX = px, oldY = py;  // Save old position

    // Move only if there is no wall
    if (IsKeyPressed(KEY_W) && !maze[px][py].topWall && py > 0) py--;
    if (IsKeyPressed(KEY_S) && !maze[px][py].bottomWall && py < GRID_H - 1) py++;
    if (IsKeyPressed(KEY_A) && !maze[px][py].leftWall && px > 0) px--;
    if (IsKeyPressed(KEY_D) && !maze[px][py].rightWall && px < GRID_W - 1) px++;

    // If moved to a coin cell → take coin
    if ((px != oldX || py != oldY) && coins[px][py]) {
        coins[px][py] = false;
        coinsCollected++;
    }

    // Start timer when player first moves
    if (!gameStarted &&
        (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_A) ||
            IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) ||
            IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_LEFT) ||
            IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_RIGHT))) {

        gameStarted = true;
        startTime = (float)GetTime();
    }
}


// ---------------- HARD MODE GAME ----------------

void StartHardGame() {

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    // Pick cell size to fit screen
    int cellSize = (sw / GRID_W < sh / GRID_H) ? sw / GRID_W : sh / GRID_H;
    int ox = (sw - cellSize * GRID_W) / 2;
    int oy = (sh - cellSize * GRID_H) / 2;

    // Build maze + coins
    InitializeMaze();
    GenerateMaze(0, 0);
    InitializeCoins();

    px = 0;
    py = 0;
    bool win = false;

    // Reset timer
    gameStarted = false;
    startTime = 0;
    elapsedTime = 0;

    // Darkness effect textures
    RenderTexture2D darkness = LoadRenderTexture(sw, sh);
    RenderTexture2D mazeTexture = LoadRenderTexture(sw, sh);
    float visionRadius = 60.0f;

    // Pre-draw maze walls (they never change)
    BeginTextureMode(mazeTexture);
    ClearBackground(BLACK);

    for (int x = 0; x < GRID_W; x++)
        for (int y = 0; y < GRID_H; y++) {

            int sx = ox + x * cellSize;
            int sy = oy + y * cellSize;

            if (maze[x][y].topWall)    DrawLine(sx, sy, sx + cellSize, sy, WHITE);
            if (maze[x][y].leftWall)   DrawLine(sx, sy, sx, sy + cellSize, WHITE);
            if (maze[x][y].rightWall)  DrawLine(sx + cellSize, sy, sx + cellSize, sy + cellSize, WHITE);
            if (maze[x][y].bottomWall) DrawLine(sx, sy + cellSize, sx + cellSize, sy + cellSize, WHITE);
        }

    // Finish block
    DrawRectangle(ox + endX * cellSize + 5, oy + endY * cellSize + 5,
        cellSize - 10, cellSize - 10, RED);

    EndTextureMode();

    while (!WindowShouldClose()) {

        // Update timer only while not won
        if (gameStarted && !win)
            elapsedTime = (float)GetTime() - startTime;

        // Move player
        if (!win) MovePlayer();

        // Check win
        if (px == endX && py == endY)
            win = true;

        // Draw darkness (light around player)
        BeginTextureMode(darkness);
        ClearBackground(BLACK);

        DrawCircleGradient(
            ox + px * cellSize + cellSize / 2,
            oy + py * cellSize + cellSize / 2,
            visionRadius,
            Fade(BLACK, 0.0f), // center = bright
            Fade(BLACK, 1.0f)  // outside = dark
        );

        EndTextureMode();

        // -------- DRAW EVERYTHING --------

        BeginDrawing();
        ClearBackground(BLACK);

        // Draw maze walls from pre-rendered texture
        DrawTextureRec(
            mazeTexture.texture,
            Rectangle{ 0, 0, (float)mazeTexture.texture.width, (float)-mazeTexture.texture.height },
            Vector2{ 0, 0 },
            WHITE
        );

        // Draw coins (these can change)
        for (int x = 0; x < GRID_W; x++)
            for (int y = 0; y < GRID_H; y++)
                if (coins[x][y])
                    DrawCircle(
                        ox + x * cellSize + cellSize / 2,
                        oy + y * cellSize + cellSize / 2,
                        cellSize / 6, YELLOW
                    );

        // Draw darkness on top
        DrawTextureRec(
            darkness.texture,
            Rectangle{ 0, 0, (float)darkness.texture.width, (float)-darkness.texture.height },
            Vector2{ 0, 0 },
            WHITE
        );

        // Draw player
        DrawCircle(
            ox + px * cellSize + cellSize / 2,
            oy + py * cellSize + cellSize / 2,
            cellSize / 3,
            YELLOW
        );

        // Draw timer
        int totalSec = (int)elapsedTime;
        int minutes = totalSec / 60;
        int seconds = totalSec % 60;

        DrawText("TIME:", 510, 0, 40, WHITE);
        DrawText(TextFormat("%02d:%02d", minutes, seconds), 630, 0, 40, WHITE);

        // Draw coins HUD
        DrawCoinCounter();

        // Win message
        if (win) {
            DrawText("YOU WIN!", sw / 2 - 150, sh / 2 - 50, 60, GOLD);
            DrawText(TextFormat("Coins: %d/%d", coinsCollected, totalCoins),
                sw / 2 - 100, sh / 2 + 20, 30, YELLOW);
            DrawText("Press ESC to return", sw / 2 - 140, sh / 2 + 60, 30, WHITE);
        }

        EndDrawing();

        // Return to menu
        if (IsKeyPressed(KEY_ESCAPE)) break;
    }

    // Free textures
    UnloadRenderTexture(darkness);
    UnloadRenderTexture(mazeTexture);
}
