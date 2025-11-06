#include "raylib.h"
#include <iostream>
#include <limits>
#include "game.h"
using namespace std;

void startNewGame() {
    cout << "Starting new Game..." << "\n";
    cout << "Press Enter to open Raylib window...";
    cin.get();

    InitWindow(800, 600, "Raylib Window - New Game");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("Raylib window is running!", 220, 280, 20, YELLOW);
        EndDrawing();
    }

    CloseWindow();

    cout << "\nGame ended. Returning to menu...\n";
    cout << "Press Enter to continue...";
    cin.get();
}
