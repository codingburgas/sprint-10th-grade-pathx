#include "raylib.h"
#include "help.h"

void ShowHelp() {
    bool running = true;
    while (running && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("HELP:\nWASD to move\nReach the red square to win\nPress ESC to return", 50, 50, 30, WHITE);
        EndDrawing();
        if (IsKeyPressed(KEY_ESCAPE)) running = false;
    }
}