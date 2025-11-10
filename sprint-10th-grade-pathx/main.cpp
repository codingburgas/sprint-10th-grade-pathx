#include "raylib.h"
#include "gameEasy.h"
#include "gameMedium.h"
#include "gameHard.h"
#include "help.h"

enum GameState { STATE_MAIN_MENU, STATE_DIFFICULTY, STATE_EXIT };

int main() {
    InitWindow(0, 0, "Maze Game Menu");
    ToggleFullscreen();
    SetTargetFPS(60);

    GameState state = STATE_MAIN_MENU;

    while (!WindowShouldClose()) {
        int SW = GetScreenWidth();
        int SH = GetScreenHeight();
        Vector2 mouse = GetMousePosition();

        Rectangle btnPlay = { SW / 2 - 150, SH / 2 - 100, 300, 60 };
        Rectangle btnHelp = { SW / 2 - 150, SH / 2,       300, 60 };
        Rectangle btnExit = { SW / 2 - 150, SH / 2 + 100, 300, 60 };
        Rectangle btnEasy = { SW / 2 - 150, SH / 2 - 120, 300, 60 };
        Rectangle btnMedium = { SW / 2 - 150, SH / 2 - 20,  300, 60 };
        Rectangle btnHard = { SW / 2 - 150, SH / 2 + 80,  300, 60 };

        switch (state) {
        case STATE_MAIN_MENU:
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, btnPlay))  state = STATE_DIFFICULTY;
                if (CheckCollisionPointRec(mouse, btnHelp))  ShowHelp();
                if (CheckCollisionPointRec(mouse, btnExit))  state = STATE_EXIT;
            }

            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("MAZE GAME", SW / 2 - 250, SH / 2 - 240, 80, WHITE);
            DrawRectangleRec(btnPlay, DARKGRAY); DrawText("PLAY", btnPlay.x + 110, btnPlay.y + 10, 40, GREEN);
            DrawRectangleRec(btnHelp, DARKGRAY); DrawText("HELP", btnHelp.x + 110, btnHelp.y + 10, 40, YELLOW);
            DrawRectangleRec(btnExit, DARKGRAY); DrawText("EXIT", btnExit.x + 110, btnExit.y + 10, 40, RED);
            EndDrawing();
            break;

        case STATE_DIFFICULTY:
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, btnEasy))   StartEasyGame();
                if (CheckCollisionPointRec(mouse, btnMedium)) StartMediumGame();
                if (CheckCollisionPointRec(mouse, btnHard))   StartHardGame();
            }
            if (IsKeyPressed(KEY_ESCAPE)) state = STATE_MAIN_MENU;

            BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawText("CHOOSE DIFFICULTY", SW / 2 - 330, SH / 2 - 260, 60, WHITE);
            DrawRectangleRec(btnEasy, DARKGRAY); DrawText("EASY", btnEasy.x + 100, btnEasy.y + 10, 40, GREEN);
            DrawRectangleRec(btnMedium, DARKGRAY); DrawText("MEDIUM", btnMedium.x + 80, btnMedium.y + 10, 40, YELLOW);
            DrawRectangleRec(btnHard, DARKGRAY); DrawText("HARD", btnHard.x + 100, btnHard.y + 10, 40, RED);
            EndDrawing();
            break;

        case STATE_EXIT:
            CloseWindow();
            return 0;
        }
    }
    CloseWindow();
    return 0;
}
