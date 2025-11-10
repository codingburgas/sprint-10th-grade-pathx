#include "raylib.h"
#include "game.h"  
#define MENU_W 900
#define MENU_H 600

enum GameState {
    STATE_MENU,
    STATE_SETTINGS,
    STATE_HELP,
    STATE_EXIT
};

int main() {
    InitWindow(MENU_W, MENU_H, "Maze Game Menu");
    SetTargetFPS(60);

    GameState state = STATE_MENU;

    Rectangle btnStart = { 350, 250, 200, 40 };
    Rectangle btnSettings = { 350, 300, 200, 40 };
    Rectangle btnHelp = { 350, 350, 200, 40 };
    Rectangle btnExit = { 350, 400, 200, 40 };

    while (!WindowShouldClose()) {

        Vector2 mouse = GetMousePosition();

        switch (state) {

        case STATE_MENU:

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

                if (CheckCollisionPointRec(mouse, btnStart)) {
                 
                    StartNewGame();

                 
                    InitWindow(MENU_W, MENU_H, "Maze Game Menu");
                    SetTargetFPS(60);
                    state = STATE_MENU;
                }

                if (CheckCollisionPointRec(mouse, btnSettings)) {
                    state = STATE_SETTINGS;
                }

                if (CheckCollisionPointRec(mouse, btnHelp)) {
                    state = STATE_HELP;
                }

                if (CheckCollisionPointRec(mouse, btnExit)) {
                    state = STATE_EXIT;
                }
            }

            BeginDrawing();
            ClearBackground(BLACK);

            DrawText("MAZE PROJECT", 240, 130, 50, WHITE);

            DrawRectangleRec(btnStart, DARKGRAY);
            DrawRectangleRec(btnSettings, DARKGRAY);
            DrawRectangleRec(btnHelp, DARKGRAY);
            DrawRectangleRec(btnExit, DARKGRAY);

            DrawText("Start New Game", btnStart.x + 15, btnStart.y + 10, 20, SKYBLUE);
            DrawText("Settings", btnSettings.x + 15, btnSettings.y + 10, 20, RAYWHITE);
            DrawText("Help", btnHelp.x + 15, btnHelp.y + 10, 20, RAYWHITE);
            DrawText("Exit", btnExit.x + 15, btnExit.y + 10, 20, RED);

            EndDrawing();
            break;

        case STATE_SETTINGS:

            BeginDrawing();
            ClearBackground(DARKGRAY);
            DrawText("SETTINGS — Press ESC to return", 150, 200, 30, WHITE);
            EndDrawing();

            if (IsKeyPressed(KEY_ESCAPE)) {
                state = STATE_MENU;
            }
            break;

        case STATE_HELP:

            BeginDrawing();
            ClearBackground(DARKGRAY);
            DrawText("HELP — Press ESC to return", 150, 200, 30, WHITE);
            EndDrawing();

            if (IsKeyPressed(KEY_ESCAPE)) {
                state = STATE_MENU;
            }
            break;

        case STATE_EXIT:
            CloseWindow();
            return 0;
        }
    }

    CloseWindow();
    return 0;
}
