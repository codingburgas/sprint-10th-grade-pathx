#include "raylib.h"
#include "game.h"
#include <cstdlib>

// Параметри hard
static const int GRID_W = 16;
static const int GRID_H = 16;

struct Cell {
    bool visited;
    bool top, bottom, left, right;
};

static Cell** CreateGrid(int w, int h) {
    Cell** g = new Cell * [w];
    for (int i = 0; i < w; ++i) {
        g[i] = new Cell[h];
        for (int j = 0; j < h; ++j) {
            g[i][j].visited = false;
            g[i][j].top = g[i][j].bottom = g[i][j].left = g[i][j].right = true;
        }
    }
    return g;
}

static void FreeGrid(Cell** g, int w) {
    for (int i = 0; i < w; ++i) delete[] g[i];
    delete[] g;
}

static void ShuffleDirs(int d[4]) {
    for (int i = 0; i < 4; ++i) {
        int r = GetRandomValue(i, 3);
        int t = d[i]; d[i] = d[r]; d[r] = t;
    }
}

static void Generate(Cell** g, int x, int y, int w, int h) {
    g[x][y].visited = true;
    int dirs[4] = { 0,1,2,3 };
    ShuffleDirs(dirs);

    for (int k = 0; k < 4; ++k) {
        int nx = x, ny = y;
        if (dirs[k] == 0) ny--;
        if (dirs[k] == 1) nx++;
        if (dirs[k] == 2) ny++;
        if (dirs[k] == 3) nx--;

        if (nx < 0 || nx >= w || ny < 0 || ny >= h) continue;
        if (g[nx][ny].visited) continue;

        if (dirs[k] == 0) { g[x][y].top = false;    g[nx][ny].bottom = false; }
        if (dirs[k] == 1) { g[x][y].right = false;  g[nx][ny].left = false; }
        if (dirs[k] == 2) { g[x][y].bottom = false; g[nx][ny].top = false; }
        if (dirs[k] == 3) { g[x][y].left = false;   g[nx][ny].right = false; }

        Generate(g, nx, ny, w, h);
    }
}

static void DrawRegion(Cell** g, int w, int h, int cellSize, int ox, int oy, int sx, int sy, int ex, int ey) {
    for (int x = sx; x < ex; ++x) {
        for (int y = sy; y < ey; ++y) {
            if (x < 0 || x >= w || y < 0 || y >= h) continue;
            int px = ox + x * cellSize;
            int py = oy + y * cellSize;
            if (g[x][y].top)    DrawLine(px, py, px + cellSize, py, WHITE);
            if (g[x][y].left)   DrawLine(px, py, px, py + cellSize, WHITE);
            if (g[x][y].right)  DrawLine(px + cellSize, py, px + cellSize, py + cellSize, WHITE);
            if (g[x][y].bottom) DrawLine(px, py + cellSize, px + cellSize, py + cellSize, WHITE);
        }
    }
}

void StartHardGame() {
    Cell** maze = CreateGrid(GRID_W, GRID_H);
    Generate(maze, 0, 0, GRID_W, GRID_H);

    int px = 0, py = 0;
    int endX = GRID_W - 1, endY = GRID_H - 1;
    bool gameWon = false;

    float flashTimer = 0.0f;
    float nextFlash = (float)GetRandomValue(10, 15);
    bool flashNow = false;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;

        float dt = GetFrameTime();
        flashTimer += dt;
        if (flashTimer >= nextFlash) {
            flashNow = true;
            flashTimer = 0.0f;
            nextFlash = (float)GetRandomValue(10, 15);
        }

        int sw = GetScreenWidth();
        int sh = GetScreenHeight();
        int cellSize = (sw / GRID_W < sh / GRID_H) ? sw / GRID_W : sh / GRID_H;
        int ox = (sw - cellSize * GRID_W) / 2;
        int oy = (sh - cellSize * GRID_H) / 2;

        // движение
        if (!gameWon) {
            if (IsKeyPressed(KEY_W) && !maze[px][py].top && py > 0)      py--;
            if (IsKeyPressed(KEY_S) && !maze[px][py].bottom && py < GRID_H - 1) py++;
            if (IsKeyPressed(KEY_A) && !maze[px][py].left && px > 0)      px--;
            if (IsKeyPressed(KEY_D) && !maze[px][py].right && px < GRID_W - 1) px++;
        }
        if (px == endX && py == endY) gameWon = true;

        BeginDrawing();
        ClearBackground(BLACK);

        if (flashNow) {
            DrawRegion(maze, GRID_W, GRID_H, cellSize, ox, oy, 0, 0, GRID_W, GRID_H);
            DrawRectangleLines(ox, oy, cellSize * GRID_W, cellSize * GRID_H, WHITE);
            DrawRectangle(ox + 5 + 0 * cellSize, oy + 5 + 0 * cellSize, cellSize - 10, cellSize - 10, GREEN);
            DrawRectangle(ox + 5 + endX * cellSize, oy + 5 + endY * cellSize, cellSize - 10, cellSize - 10, RED);
            DrawCircle(ox + px * cellSize + cellSize / 2, oy + py * cellSize + cellSize / 2, cellSize / 3, YELLOW);
            flashNow = false;
        }
        else {

            DrawRegion(maze, GRID_W, GRID_H, cellSize, ox, oy, 0, 0, GRID_W, GRID_H);
            DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.98f));
            int r = 1;
            int sx = px - r, sy = py - r, ex = px + r + 1, ey = py + r + 1;
            DrawRegion(maze, GRID_W, GRID_H, cellSize, ox, oy, sx, sy, ex, ey);
            DrawRectangle(ox + 5 + 0 * cellSize, oy + 5 + 0 * cellSize, cellSize - 10, cellSize - 10, GREEN);
            DrawRectangle(ox + 5 + endX * cellSize, oy + 5 + endY * cellSize, cellSize - 10, cellSize - 10, RED);
            DrawCircle(ox + px * cellSize + cellSize / 2, oy + py * cellSize + cellSize / 2, cellSize / 3, YELLOW);
        }

        if (gameWon) {
            DrawText("YOU WIN!", sw / 2 - 150, sh / 2 - 50, 60, GOLD);
            DrawText("Press ESC to return", sw / 2 - 140, sh / 2 + 20, 30, RAYWHITE);
        }

        EndDrawing();
    }

    FreeGrid(maze, GRID_W);
}
