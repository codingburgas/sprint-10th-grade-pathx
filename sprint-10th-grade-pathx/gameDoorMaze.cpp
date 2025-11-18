#include "gameDoorMaze.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

// Maze grid
static Cell maze[GRID_H][GRID_W];

// Player instance
static Player player;

// Ending cell
static int endX = GRID_W - 1;
static int endY = GRID_H - 1;

// Time tracking
static float startTime = 0.0f;
static float elapsedTime = 0.0f;

// Sounds
static Sound itemSound;
static Sound winSound;
static Sound coinSound;
static Sound keyUseSound;

static bool coins[GRID_H][GRID_W] = { false };
static int coinsCollected = 0;
static int totalCoins = 0;

static int lastGameCoins = 0;

int GetDoorMazeCoins() { return lastGameCoins; }

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

static void InitializeMaze() {
    for (int y = 0; y < GRID_H; y++)
        for (int x = 0; x < GRID_W; x++)
            maze[y][x] = Cell{};
}

static void PlaceDoorsAndItems() {
    maze[3][3].hasDoor = true;
    maze[3][3].doorColor = 1;
    maze[1][1].item = KEY_RED;
    maze[2][5].item = KEY_BLUE;
    maze[4][2].item = KEY_GREEN;
    maze[5][5].item = UPGRADE_LIGHT;
}

static void GenerateMazeDFS(int x, int y) {
    bool visited[GRID_H][GRID_W] = { false };
    visited[y][x] = true;

    std::vector<Vector2> stack;
    stack.push_back({ (float)x, (float)y });

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

            stack.push_back({ (float)cx,(float)cy });
            visited[cy][cx] = true;
        }
        else {
            stack.pop_back();
        }
    }
}

static void DrawCell(int x, int y, int cellSize, int ox, int oy) {
    int sx = ox + x * cellSize;
    int sy = oy + y * cellSize;

    if (maze[y][x].topWall) DrawLine(sx, sy, sx + cellSize, sy, WHITE);
    if (maze[y][x].leftWall) DrawLine(sx, sy, sx, sy + cellSize, WHITE);
    if (maze[y][x].rightWall) DrawLine(sx + cellSize, sy, sx + cellSize, sy + cellSize, WHITE);
    if (maze[y][x].bottomWall) DrawLine(sx, sy + cellSize, sx + cellSize, sy + cellSize, WHITE);

    if (maze[y][x].hasDoor) {
        Color c = RED;
        if (maze[y][x].doorColor == 2) c = BLUE;
        if (maze[y][x].doorColor == 3) c = GREEN;
        DrawRectangle(sx + 2, sy + 2, cellSize - 4, cellSize - 4, c);
    }

    if (maze[y][x].item != NONE) {
        Color c = YELLOW;
        if (maze[y][x].item == KEY_RED) c = RED;
        if (maze[y][x].item == KEY_BLUE) c = BLUE;
        if (maze[y][x].item == KEY_GREEN) c = GREEN;
        DrawCircle(sx + cellSize / 2, sy + cellSize / 2, cellSize / 4, c);
    }

    if (coins[y][x]) DrawCircle(sx + cellSize / 2, sy + cellSize / 2, cellSize / 6, YELLOW);
}

static void DrawMaze(int cellSize, int ox, int oy) {
    for (int y = 0; y < GRID_H; y++)
        for (int x = 0; x < GRID_W; x++) {
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

static void DrawHUD() {
    // coins
    DrawRectangle(10, 10, 150, 30, Fade(BLACK, 0.7f));
    DrawText("COINS:", 15, 15, 20, WHITE);
    DrawText(TextFormat("%d/%d", coinsCollected, totalCoins), 80, 15, 20, YELLOW);

    // keys
    DrawRectangle(200, 10, 200, 30, Fade(BLACK, 0.7f));
    DrawText(TextFormat("RED: %d", player.keys[0] ? 1 : 0), 205, 15, 20, RED);
    DrawText(TextFormat("BLUE: %d", player.keys[1] ? 1 : 0), 265, 15, 20, BLUE);
    DrawText(TextFormat("GREEN: %d", player.keys[2] ? 1 : 0), 335, 15, 20, GREEN);
}

void StartGameDoorMaze() {
    srand((unsigned int)time(0));

    itemSound = LoadSound("item.wav"); SetSoundVolume(itemSound, 1.0f);
    winSound = LoadSound("win.wav"); SetSoundVolume(winSound, 1.0f);
    coinSound = LoadSound("coins.wav"); SetSoundVolume(coinSound, 1.0f);
    keyUseSound = LoadSound("keyusesound.wav"); SetSoundVolume(keyUseSound, 1.0f);

    InitializeMaze();
    GenerateMazeDFS(0, 0);
    PlaceDoorsAndItems();
    InitializeCoins();

    const int screenWidth = 640, screenHeight = 640;
    InitWindow(screenWidth, screenHeight, "GameDoorMaze");
    SetTargetFPS(60);
    int cellSize = screenWidth / GRID_W;
    int offsetX = (screenWidth - cellSize * GRID_W) / 2;
    int offsetY = (screenHeight - cellSize * GRID_H) / 2;

    player = Player{};
    startTime = (float)GetTime();
    lastGameCoins = 0;

    while (!WindowShouldClose()) {
        elapsedTime = (float)GetTime() - startTime;
        int oldX = player.x, oldY = player.y;

        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && !maze[player.y][player.x].topWall) player.y--;
        if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && !maze[player.y][player.x].bottomWall) player.y++;
        if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && !maze[player.y][player.x].leftWall) player.x--;
        if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && !maze[player.y][player.x].rightWall) player.x++;

        // Collect coins
        if ((player.x != oldX || player.y != oldY) && coins[player.y][player.x]) {
            coins[player.y][player.x] = false;
            coinsCollected++;
            PlaySound(coinSound);
        }

        // Doors
        if (maze[player.y][player.x].hasDoor) {
            int color = maze[player.y][player.x].doorColor;
            if (player.keys[color - 1]) {
                player.keys[color - 1] = false;
                PlaySound(keyUseSound);
                maze[player.y][player.x].hasDoor = false;
            }
            else { player.x = oldX; player.y = oldY; }
        }

        // Items
        if (maze[player.y][player.x].item != NONE) {
            if (maze[player.y][player.x].item == KEY_RED) player.keys[0] = true;
            if (maze[player.y][player.x].item == KEY_BLUE) player.keys[1] = true;
            if (maze[player.y][player.x].item == KEY_GREEN) player.keys[2] = true;
            if (maze[player.y][player.x].item == UPGRADE_LIGHT) player.visionRadius++;
            maze[player.y][player.x].item = NONE;
            PlaySound(itemSound);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawMaze(cellSize, offsetX, offsetY);
        DrawHUD();
        DrawText(TextFormat("Time: %.1f", elapsedTime), 10, 50, 20, WHITE);
        EndDrawing();

        bool allKeysUsed = !player.keys[0] && !player.keys[1] && !player.keys[2];
        if (player.x == endX && player.y == endY && allKeysUsed) {
            lastGameCoins = coinsCollected;
            PlaySound(winSound);
            break;
        }
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("YOU WIN!", 200, 200, 50, GOLD);
        DrawText(TextFormat("Time: %.1f", elapsedTime), 200, 270, 30, WHITE);
        DrawText(TextFormat("Coins: %d/%d", coinsCollected, totalCoins), 200, 310, 30, YELLOW);
        DrawText("Press ESC to Exit", 200, 360, 20, WHITE);
        EndDrawing();
        if (IsKeyPressed(KEY_ESCAPE)) break;
    }

    UnloadSound(itemSound);
    UnloadSound(winSound);
    UnloadSound(coinSound);
    UnloadSound(keyUseSound);
    CloseWindow();
}