#include "gameDoorMaze.h"
#include <vector>
#include <cstdlib>
#include <ctime>

// Maze grid
static Cell maze[GRID_H][GRID_W];

// Player instance
static Player player;

// Ending cell (goal)
static int endX = GRID_W - 1;
static int endY = GRID_H - 1;

// Time tracking
static float startTime = 0.0f;
static float elapsedTime = 0.0f;

// Initialize all maze cells to default values
static void InitializeMaze() {
    for (int y = 0; y < GRID_H; y++)
        for (int x = 0; x < GRID_W; x++)
            maze[y][x] = Cell{};
}

// Place predefined doors and items in the maze
static void PlaceDoorsAndItems() {
    maze[3][3].hasDoor = true;
    maze[3][3].doorColor = 1; // red door

    maze[1][1].item = KEY_RED;       // red key placed at (1,1)
    maze[5][5].item = UPGRADE_LIGHT; // vision radius upgrade at (5,5)
}

// DFS maze generator
static void GenerateMazeDFS(int x, int y) {
    bool visited[GRID_H][GRID_W] = { false };
    visited[y][x] = true;

    std::vector<Vector2> stack;
    stack.push_back({ (float)x, (float)y });

    while (!stack.empty()) {
        Vector2 cell = stack.back();
        int cx = (int)cell.x;
        int cy = (int)cell.y;

        // Find possible unvisited neighbors
        std::vector<int> neighbors;
        if (cy > 0 && !visited[cy - 1][cx]) neighbors.push_back(0);
        if (cx < GRID_W - 1 && !visited[cy][cx + 1]) neighbors.push_back(1);
        if (cy < GRID_H - 1 && !visited[cy + 1][cx]) neighbors.push_back(2);
        if (cx > 0 && !visited[cy][cx - 1]) neighbors.push_back(3);

        if (!neighbors.empty()) {
            // Pick random direction
            int dir = neighbors[rand() % neighbors.size()];

            // Remove walls between current cell and chosen neighbor
            if (dir == 0) {
                maze[cy][cx].topWall = false;
                maze[cy - 1][cx].bottomWall = false;
                cy--;
            }
            else if (dir == 1) {
                maze[cy][cx].rightWall = false;
                maze[cy][cx + 1].leftWall = false;
                cx++;
            }
            else if (dir == 2) {
                maze[cy][cx].bottomWall = false;
                maze[cy + 1][cx].topWall = false;
                cy++;
            }
            else if (dir == 3) {
                maze[cy][cx].leftWall = false;
                maze[cy][cx - 1].rightWall = false;
                cx--;
            }

            stack.push_back({ (float)cx,(float)cy });
            visited[cy][cx] = true;
        }
        else {
            // Backtrack
            stack.pop_back();
        }
    }
}

// Draw a single maze cell
static void DrawCell(int x, int y, int cellSize, int ox, int oy) {
    int sx = ox + x * cellSize;
    int sy = oy + y * cellSize;

    // Draw walls
    if (maze[y][x].topWall)    DrawLine(sx, sy, sx + cellSize, sy, WHITE);
    if (maze[y][x].leftWall)   DrawLine(sx, sy, sx, sy + cellSize, WHITE);
    if (maze[y][x].rightWall)  DrawLine(sx + cellSize, sy, sx + cellSize, sy + cellSize, WHITE);
    if (maze[y][x].bottomWall) DrawLine(sx, sy + cellSize, sx + cellSize, sy + cellSize, WHITE);

    // Draw door if present
    if (maze[y][x].hasDoor) {
        Color c = RED;
        if (maze[y][x].doorColor == 2) c = BLUE;
        if (maze[y][x].doorColor == 3) c = GREEN;
        DrawRectangle(sx + 2, sy + 2, cellSize - 4, cellSize - 4, c);
    }

    // Draw item
    if (maze[y][x].item != NONE) {
        Color c = YELLOW;
        if (maze[y][x].item == KEY_RED)   c = RED;
        if (maze[y][x].item == KEY_BLUE)  c = BLUE;
        if (maze[y][x].item == KEY_GREEN) c = GREEN;
        DrawCircle(sx + cellSize / 2, sy + cellSize / 2, cellSize / 4, c);
    }
}

// Draw the visible part of the maze based on vision radius
static void DrawMaze(int cellSize, int ox, int oy) {
    for (int y = 0; y < GRID_H; y++)
        for (int x = 0; x < GRID_W; x++) {

            int dx = abs(x - player.x);
            int dy = abs(y - player.y);

            // Only draw cells within vision radius, else draw black
            if (dx + dy <= player.visionRadius)
                DrawCell(x, y, cellSize, ox, oy);
            else
                DrawRectangle(ox + x * cellSize, oy + y * cellSize, cellSize, cellSize, BLACK);
        }

    // Draw goal cell (green)
    DrawRectangle(ox + endX * cellSize, oy + endY * cellSize, cellSize, cellSize, GREEN);

    // Draw player
    DrawRectangle(ox + player.x * cellSize, oy + player.y * cellSize, cellSize, cellSize, YELLOW);
}

void StartGameDoorMaze() {
    srand((unsigned int)time(0));

    // Setup maze
    InitializeMaze();
    GenerateMazeDFS(0, 0);
    PlaceDoorsAndItems();

    // Window setup
    const int screenWidth = 640;
    const int screenHeight = 640;
    InitWindow(screenWidth, screenHeight, "GameDoorMaze");
    SetTargetFPS(60);

    int cellSize = screenWidth / GRID_W;
    int offsetX = (screenWidth - cellSize * GRID_W) / 2;
    int offsetY = (screenHeight - cellSize * GRID_H) / 2;

    // Initialize player
    player = Player{};
    startTime = (float)GetTime();
    elapsedTime = 0.0f;

    while (!WindowShouldClose()) {
        // Update time
        elapsedTime = (float)GetTime() - startTime;

        int oldX = player.x;
        int oldY = player.y;

        // Player movement, restricted by walls
        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && !maze[player.y][player.x].topWall) player.y--;
        if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && !maze[player.y][player.x].bottomWall) player.y++;
        if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && !maze[player.y][player.x].leftWall) player.x--;
        if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && !maze[player.y][player.x].rightWall) player.x++;

        // Check if player hit a locked door
        if (maze[player.y][player.x].hasDoor) {
            int color = maze[player.y][player.x].doorColor;

            // If player doesn't have the key → revert movement
            if (!player.keys[color - 1]) {
                player.x = oldX;
                player.y = oldY;
            }
        }

        // Item pickup logic
        if (maze[player.y][player.x].item != NONE) {
            if (maze[player.y][player.x].item == KEY_RED)   player.keys[0] = true;
            if (maze[player.y][player.x].item == KEY_BLUE)  player.keys[1] = true;
            if (maze[player.y][player.x].item == KEY_GREEN) player.keys[2] = true;
            if (maze[player.y][player.x].item == UPGRADE_LIGHT) player.visionRadius++;

            maze[player.y][player.x].item = NONE; // remove item after pickup
        }

        // Draw everything
        BeginDrawing();
        ClearBackground(BLACK);
        DrawMaze(cellSize, offsetX, offsetY);

        DrawText(TextFormat("Time: %.1f", elapsedTime), 10, 10, 20, WHITE);
        DrawText(TextFormat("R:%d B:%d G:%d", player.keys[0], player.keys[1], player.keys[2]), 10, 40, 20, WHITE);
        EndDrawing();

        // Win condition
        if (player.x == endX && player.y == endY) break;
    }

    // End screen loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("YOU WIN!", 200, 200, 50, GOLD);
        DrawText(TextFormat("Time: %.1f", elapsedTime), 200, 300, 30, WHITE);
        DrawText("Press ESC to Exit", 200, 400, 20, WHITE);

        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE)) break;
    }

    CloseWindow();
}
