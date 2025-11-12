#include "raylib.h"

#define GRID_W 16
#define GRID_H 16

struct Cell {
    bool visited, topWall, bottomWall, leftWall, rightWall;
};

static Cell maze[GRID_W][GRID_H];
static int px = 0, py = 0, endX = GRID_W - 1, endY = GRID_H - 1;

// Timer variables
static bool gameStarted = false;
static float startTime = 0.0f;
static float elapsedTime = 0.0f;

static void InitializeMaze() {
    for (int x = 0; x < GRID_W; x++)
        for (int y = 0; y < GRID_H; y++)
            maze[x][y] = Cell{ false, true, true, true, true };
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

        if (dirs[i] == 0) { maze[x][y].topWall = false; maze[nx][ny].bottomWall = false; }
        if (dirs[i] == 1) { maze[x][y].rightWall = false; maze[nx][ny].leftWall = false; }
        if (dirs[i] == 2) { maze[x][y].bottomWall = false; maze[nx][ny].topWall = false; }
        if (dirs[i] == 3) { maze[x][y].leftWall = false; maze[nx][ny].rightWall = false; }

        GenerateMaze(nx, ny);
    }
}

static void DrawMaze(int cellSize, int ox, int oy) {
    for (int x = 0; x < GRID_W; x++)
        for (int y = 0; y < GRID_H; y++) {
            int sx = ox + x * cellSize;
            int sy = oy + y * cellSize;
            if (maze[x][y].topWall) DrawLine(sx, sy, sx + cellSize, sy, WHITE);
            if (maze[x][y].leftWall) DrawLine(sx, sy, sx, sy + cellSize, WHITE);
            if (maze[x][y].rightWall) DrawLine(sx + cellSize, sy, sx + cellSize, sy + cellSize, WHITE);
            if (maze[x][y].bottomWall) DrawLine(sx, sy + cellSize, sx + cellSize, sy + cellSize, WHITE);
        }

    DrawRectangle(ox + endX * cellSize + 5, oy + endY * cellSize + 5, cellSize - 10, cellSize - 10, RED);
}

static void MovePlayer() {
    if (IsKeyPressed(KEY_W) && !maze[px][py].topWall && py > 0) py--;
    if (IsKeyPressed(KEY_S) && !maze[px][py].bottomWall && py < GRID_H - 1) py++;
    if (IsKeyPressed(KEY_A) && !maze[px][py].leftWall && px > 0) px--;
    if (IsKeyPressed(KEY_D) && !maze[px][py].rightWall && px < GRID_W - 1) px++;

    // Start the game timer on first movement
    if (!gameStarted && (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) ||
        IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_RIGHT))) {
        gameStarted = true;
        startTime = (float)GetTime();
    }
}

void StartHardGame() {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    int cellSize = (sw / GRID_W < sh / GRID_H) ? sw / GRID_W : sh / GRID_H;
    int ox = (sw - cellSize * GRID_W) / 2;
    int oy = (sh - cellSize * GRID_H) / 2;

    InitializeMaze();
    GenerateMaze(0, 0);

    px = 0; py = 0;
    bool win = false;

    // Reset timer variables
    gameStarted = false;
    startTime = 0.0f;
    elapsedTime = 0.0f;

    RenderTexture2D darkness = LoadRenderTexture(sw, sh);
    RenderTexture2D mazeTexture = LoadRenderTexture(sw, sh);
    float visionRadius = 60.0f;

    BeginTextureMode(mazeTexture);
    ClearBackground(BLACK);
    DrawMaze(cellSize, ox, oy);
    EndTextureMode();

    while (!WindowShouldClose()) {
        // Update game timer if game has started
        if (gameStarted && !win) {
            elapsedTime = (float)GetTime() - startTime;
        }

        if (!win) MovePlayer();
        if (px == endX && py == endY) win = true;

        BeginTextureMode(darkness);
        ClearBackground(BLACK);
        DrawCircleGradient(
            ox + px * cellSize + cellSize / 2,
            oy + py * cellSize + cellSize / 2,
            visionRadius,
            Fade(BLACK, 0.0f),
            Fade(BLACK, 1.0f)
        );
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTextureRec(
            mazeTexture.texture,
            Rectangle{ 0, 0, (float)mazeTexture.texture.width, (float)-mazeTexture.texture.height },
            Vector2{ 0, 0 },
            WHITE
        );

        DrawTextureRec(
            darkness.texture,
            Rectangle{ 0, 0, (float)darkness.texture.width, (float)-darkness.texture.height },
            Vector2{ 0, 0 },
            WHITE
        );
        DrawCircle(ox + px * cellSize + cellSize / 2, oy + py * cellSize + cellSize / 2, cellSize / 3, YELLOW);

        // Format and display the timer
        int totalSeconds = (int)elapsedTime;
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;

        DrawText("TIME:", sw / 2 - 160, 20, 40, WHITE);
        DrawText(TextFormat("%02d:%02d", minutes, seconds), sw / 2 - 50, 20, 40, WHITE);

        if (win) {
            DrawText("YOU WIN!", sw / 2 - 150, sh / 2 - 50, 60, GOLD);
            DrawText("Press ESC to return", sw / 2 - 140, sh / 2 + 20, 30, RAYWHITE);
        }

        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE)) break;
    }

    UnloadRenderTexture(darkness);
    UnloadRenderTexture(mazeTexture);
}