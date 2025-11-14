#include "raylib.h"
#include "gameEasy.h"

#define GRID_W 16
#define GRID_H 16

struct Cell {
    bool visited, topWall, bottomWall, leftWall, rightWall;
};

static Cell maze[GRID_W][GRID_H];

static int px = 0, py = 0;
static int endX = GRID_W - 1, endY = GRID_H - 1;

static bool gameStarted = false;
static bool reachedEnd = false;

static float startTime = 0.0f;
static float elapsedTime = 0.0f;

static bool coins[GRID_W][GRID_H];
static int coinsCollected = 0;
static int totalCoins = 0;

enum GameState {
    GAME_PLAYING,
    GAME_WINSCREEN
};
static GameState state = GAME_PLAYING;

// --------- MAZE ----------
static void InitializeMaze() {
    for (int x = 0; x < GRID_W; x++)
        for (int y = 0; y < GRID_H; y++)
            maze[x][y] = { false,true,true,true,true };
}

static void ShuffleDirs(int d[4]) {
    for (int i = 0; i < 4; i++) {
        int r = GetRandomValue(i, 3);
        int t = d[i]; d[i] = d[r]; d[r] = t;
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

// --------- COINS ----------
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

// --------- DRAW ----------
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

    DrawRectangle(ox + endX * cellSize + 5, oy + endY * cellSize + 5, cellSize - 10, cellSize - 10, RED);
}

// --------- MOVEMENT ----------
static void MovePlayer() {
    if (IsKeyPressed(KEY_W) && !maze[px][py].topWall && py > 0)    py--;
    if (IsKeyPressed(KEY_S) && !maze[px][py].bottomWall && py < GRID_H - 1) py++;
    if (IsKeyPressed(KEY_A) && !maze[px][py].leftWall && px > 0)   px--;
    if (IsKeyPressed(KEY_D) && !maze[px][py].rightWall && px < GRID_W - 1)  px++;

    if (coins[px][py]) {
        coins[px][py] = false;
        coinsCollected++;
    }
}

// ======================================================
//                       MAIN GAME
// ======================================================
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
            }

            if (gameStarted)
                elapsedTime = GetTime() - startTime;

            DrawMaze(cellSize, ox, oy);
            DrawCircle((float)(ox + px * cellSize + cellSize / 2),
                (float)(oy + py * cellSize + cellSize / 2),
                cellSize / 3.0f, GREEN);

            DrawText(TextFormat("Time: %.1f", elapsedTime), 20, 20, 30, WHITE);
            DrawText(TextFormat("Coins: %d/%d", coinsCollected, totalCoins), 20, 60, 30, YELLOW);
        }

        if (state == GAME_WINSCREEN) {
            DrawText("YOU WIN!", sw / 2 - 150, sh / 2 - 180, 60, GOLD);
            DrawText(TextFormat("Time: %.1f sec", elapsedTime), sw / 2 - 100, sh / 2 - 100, 40, WHITE);
            DrawText(TextFormat("Coins: %d/%d", coinsCollected, totalCoins), sw / 2 - 100, sh / 2 - 50, 40, YELLOW);

            DrawText("Press R to Play Again", sw / 2 - 160, sh / 2 + 20, 30, RAYWHITE);
            DrawText("Press ESC for Menu", sw / 2 - 150, sh / 2 + 60, 30, RAYWHITE);

            if (IsKeyPressed(KEY_R)) {
                // reset game state instead of recursive call
                InitializeMaze();
                GenerateMaze(0, 0);
                InitializeCoins();
                px = py = 0;
                gameStarted = false;
                reachedEnd = false;
                state = GAME_PLAYING;
                startTime = 0.0f;
                elapsedTime = 0.0f;
            }

            if (IsKeyPressed(KEY_ESCAPE)) {
                break; // exit game loop
            }
        }

        EndDrawing();
    }
}
