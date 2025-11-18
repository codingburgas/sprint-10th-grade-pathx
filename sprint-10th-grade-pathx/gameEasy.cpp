#include "raylib.h"
#include "gameEasy.h"

#define GRID_W 16
#define GRID_H 16

struct Cell {
    bool visited;
    bool topWall, bottomWall, leftWall, rightWall;
};

static Cell maze[GRID_W][GRID_H];
static int px = 0, py = 0;
static int endX = GRID_W - 1;
static int endY = GRID_H - 1;

static bool gameStarted = false;
static bool reachedEnd = false;
static float startTime = 0.0f;
static float elapsedTime = 0.0f;

static bool coins[GRID_W][GRID_H];
static int coinsCollected = 0;
static int totalCoins = 0;

static Sound coinSound;
static Sound winSound;

enum GameState {
    GAME_PLAYING,
    GAME_WINSCREEN
};
static GameState state = GAME_PLAYING;

// Global variable to return coins to main menu
static int lastGameCoins = 0;

int GetEasyGameCoins() {
    return lastGameCoins;
}

static void InitializeMaze() {
    for (int x = 0; x < GRID_W; x++)
        for (int y = 0; y < GRID_H; y++)
            maze[x][y] = { false, true, true, true, true };
}

static void ShuffleDirs(int d[4]) {
    for (int i = 0; i < 4; i++) {
        int r = GetRandomValue(i, 3);
        int t = d[i];
        d[i] = d[r];
        d[r] = t;
    }
}

static void GenerateMaze(int x, int y) {
    maze[x][y].visited = true;

    int dirs[4] = { 0,1,2,3 };
    ShuffleDirs(dirs);

    for (int i = 0; i < 4; i++) {
        int nx = x, ny = y;
        if (dirs[i] == 0) ny--;
        if (dirs[i] == 1) nx++;
        if (dirs[i] == 2) ny++;
        if (dirs[i] == 3) nx--;

        if (nx < 0 || nx >= GRID_W || ny < 0 || ny >= GRID_H) continue;
        if (maze[nx][ny].visited) continue;

        if (dirs[i] == 0) { maze[x][y].topWall = false;    maze[nx][ny].bottomWall = false; }
        if (dirs[i] == 1) { maze[x][y].rightWall = false;  maze[nx][ny].leftWall = false; }
        if (dirs[i] == 2) { maze[x][y].bottomWall = false; maze[nx][ny].topWall = false; }
        if (dirs[i] == 3) { maze[x][y].leftWall = false;   maze[nx][ny].rightWall = false; }

        GenerateMaze(nx, ny);
    }
}

static void InitializeCoins() {
    totalCoins = 0;
    coinsCollected = 0;

    for (int x = 0; x < GRID_W; x++)
        for (int y = 0; y < GRID_H; y++)
            coins[x][y] = false;

    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            if ((x == 0 && y == 0) || (x == endX && y == endY)) continue;
            if (GetRandomValue(0, 9) == 0) {
                coins[x][y] = true;
                totalCoins++;
            }
        }
    }
}

static void DrawMaze(int cellSize, int ox, int oy) {
    for (int x = 0; x < GRID_W; x++)
        for (int y = 0; y < GRID_H; y++) {
            int sx = ox + x * cellSize;
            int sy = oy + y * cellSize;

            if (maze[x][y].topWall)    DrawLine(sx, sy, sx + cellSize, sy, WHITE);
            if (maze[x][y].leftWall)   DrawLine(sx, sy, sx, sy + cellSize, WHITE);
            if (maze[x][y].rightWall)  DrawLine(sx + cellSize, sy, sx + cellSize, sy + cellSize, WHITE);
            if (maze[x][y].bottomWall) DrawLine(sx, sy + cellSize, sx + cellSize, sy + cellSize, WHITE);

            if (coins[x][y])
                DrawCircle(sx + cellSize / 2, sy + cellSize / 2, cellSize / 6.0f, YELLOW);
        }

    DrawRectangle(ox + endX * cellSize + 5, oy + endY * cellSize + 5,
        cellSize - 10, cellSize - 10, RED);
}

static void MovePlayer() {
    if (IsKeyPressed(KEY_W) && !maze[px][py].topWall && py > 0)          py--;
    if (IsKeyPressed(KEY_S) && !maze[px][py].bottomWall && py < GRID_H - 1) py++;
    if (IsKeyPressed(KEY_A) && !maze[px][py].leftWall && px > 0)          px--;
    if (IsKeyPressed(KEY_D) && !maze[px][py].rightWall && px < GRID_W - 1) px++;

    if (coins[px][py]) {
        coins[px][py] = false;
        coinsCollected++;
        PlaySound(coinSound);
    }
}

void StartEasyGame() {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    int cellSize = (sw / GRID_W < sh / GRID_H) ? sw / GRID_W : sh / GRID_H;
    int ox = (sw - cellSize * GRID_W) / 2;
    int oy = (sh - cellSize * GRID_H) / 2;

    InitializeMaze();
    GenerateMaze(0, 0);
    InitializeCoins();

    px = py = 0;
    gameStarted = false;
    reachedEnd = false;
    state = GAME_PLAYING;
    lastGameCoins = 0; // Reset for this session

    coinSound = LoadSound("coins.wav");
    SetSoundVolume(coinSound, 1.0f);
    winSound = LoadSound("win.wav");
    SetSoundVolume(winSound, 1.0f);

    float winStartTime = 0.0f; // For animated win screen

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (state == GAME_PLAYING) {
            if (!gameStarted &&
                (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_A) ||
                    IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D))) {
                gameStarted = true;
                startTime = GetTime();
            }

            MovePlayer();

            if (px == endX && py == endY) {
                reachedEnd = true;
                state = GAME_WINSCREEN;
                elapsedTime = GetTime() - startTime;
                lastGameCoins = coinsCollected; // Store coins collected in this game
                PlaySound(winSound);
                winStartTime = GetTime(); // Start animation timer
            }

            if (gameStarted)
                elapsedTime = GetTime() - startTime;

            DrawMaze(cellSize, ox, oy);
            DrawCircle(ox + px * cellSize + cellSize / 2,
                oy + py * cellSize + cellSize / 2,
                cellSize / 3.0f, GREEN);

            DrawText(TextFormat("Time: %.1f", elapsedTime), 20, 20, 30, WHITE);
            DrawText(TextFormat("Coins: %d/%d", coinsCollected, totalCoins), 20, 60, 30, YELLOW);
        }

        if (state == GAME_WINSCREEN) {
            float elapsedWin = GetTime() - winStartTime;


            float winY = sh / 2 - 180 - 50 * (elapsedWin / 3.0f); // Moves up 50 pixels in 2 seconds
            if (elapsedWin >= 0.0f)
                DrawText("YOU WIN!", sw / 2 - 150, (int)winY, 60, GOLD);

            if (elapsedWin >= 1.0f)
                DrawText(TextFormat("Coins: %d/%d", coinsCollected, totalCoins),
                    sw / 2 - 100, sh / 2 - 50, 40, YELLOW);

            if (elapsedWin >= 0.5f) //тime of appearance
                DrawText(TextFormat("Time: %.1f sec", elapsedTime),
                    sw / 2 - 100, sh / 2 - 100, 40, WHITE);

            if (elapsedWin >= 1.5f) {
                DrawText("Press R to Play Again", sw / 2 - 160, sh / 2 + 20, 30, RAYWHITE);
                DrawText("Press ESC for Menu", sw / 2 - 150, sh / 2 + 60, 30, RAYWHITE);
            }

            // Restart or exit
            if (IsKeyPressed(KEY_R)) {
                InitializeMaze();
                GenerateMaze(0, 0);
                InitializeCoins();
                px = py = 0;
                gameStarted = false;
                reachedEnd = false;
                state = GAME_PLAYING;
                startTime = elapsedTime = 0.0f;
                lastGameCoins = 0; // Reset for new game
            }

            if (IsKeyPressed(KEY_ESCAPE))
                break;
        }

        EndDrawing();
    }

    UnloadSound(coinSound);
    UnloadSound(winSound);
}