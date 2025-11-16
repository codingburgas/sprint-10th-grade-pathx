#include "raylib.h"
#include "gameEasy.h"
#include "gameMedium.h"
#include "gameHard.h"
#include "help.h"
#include "gameDoorMaze.h"
#include "gameSecMaze.h" 

enum GameState { STATE_MAIN_MENU, STATE_DIFFICULTY, STATE_EXIT };

int main() {
    int monitor = GetCurrentMonitor();
    int SW_init = GetMonitorWidth(monitor);
    int SH_init = GetMonitorHeight(monitor);

    InitWindow(SW_init, SH_init, "Maze Game");
    SetWindowState(FLAG_FULLSCREEN_MODE);
    SetTargetFPS(60);

    GameState state = STATE_MAIN_MENU;

    const float btnWidth = 300.0f;
    const float btnHeight = 60.0f;
    const float gap = 40.0f;

    const int btnCount = 5;   // EASY, MEDIUM, HARD, DOOR GAME, ESCAPE

    while (!WindowShouldClose()) {
        int SW = GetScreenWidth();
        int SH = GetScreenHeight();

        Vector2 mouse = GetMousePosition();

        // Main menu
        Rectangle btnPlay = { SW / 2.0f - btnWidth / 2, SH / 2 - 100, btnWidth, btnHeight };
        Rectangle btnHelp = { SW / 2.0f - btnWidth / 2, SH / 2.0f,      btnWidth, btnHeight };
        Rectangle btnExit = { SW / 2.0f - btnWidth / 2, SH / 2 + 100, btnWidth, btnHeight };

        // Difficulty buttons
        float startY = SH / 2.0f - (btnCount * btnHeight + (btnCount - 1) * gap) / 2.0f;

        Rectangle difficultyButtons[btnCount] = {
          { SW / 2.0f - btnWidth / 2, startY + 0 * (btnHeight + gap), btnWidth, btnHeight },
          { SW / 2.0f - btnWidth / 2, startY + 1 * (btnHeight + gap), btnWidth, btnHeight },
          { SW / 2.0f - btnWidth / 2, startY + 2 * (btnHeight + gap), btnWidth, btnHeight },
          { SW / 2.0f - btnWidth / 2, startY + 3 * (btnHeight + gap), btnWidth, btnHeight },
          { SW / 2.0f - btnWidth / 2, startY + 4 * (btnHeight + gap), btnWidth, btnHeight }  // ESCAPE
        };

        switch (state) {

        case STATE_MAIN_MENU: {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, btnHelp)) ShowHelp();
                if (CheckCollisionPointRec(mouse, btnExit)) state = STATE_EXIT;
                if (CheckCollisionPointRec(mouse, btnPlay)) state = STATE_DIFFICULTY;
            }

            BeginDrawing();
            ClearBackground(BLACK);

            DrawText("MAZE GAME", SW / 2 - 250, SH / 2 - 240, 80, WHITE);

            DrawRectangleRec(btnPlay, DARKGRAY);
            DrawText("PLAY", btnPlay.x + 110, btnPlay.y + 10, 40, GREEN);

            DrawRectangleRec(btnHelp, DARKGRAY);
            DrawText("HELP", btnHelp.x + 110, btnHelp.y + 10, 40, YELLOW);

            DrawRectangleRec(btnExit, DARKGRAY);
            DrawText("EXIT", btnExit.x + 110, btnExit.y + 10, 40, RED);

            EndDrawing();
        } break;


        case STATE_DIFFICULTY: {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

                if (CheckCollisionPointRec(mouse, difficultyButtons[0])) StartEasyGame();
                if (CheckCollisionPointRec(mouse, difficultyButtons[1])) StartMediumGame();
                if (CheckCollisionPointRec(mouse, difficultyButtons[2])) StartHardGame();
                if (CheckCollisionPointRec(mouse, difficultyButtons[3])) StartGameDoorMaze();
                if (CheckCollisionPointRec(mouse, difficultyButtons[4])) StartGameSecMaze(); // NEW ESCAPE MODE
            }

            if (IsKeyPressed(KEY_ESCAPE)) state = STATE_MAIN_MENU;

            BeginDrawing();
            ClearBackground(DARKBLUE);

            DrawText("CHOOSE GAME MODE", SW / 2 - 330, SH / 2 - 260, 60, WHITE);

            const char* names[btnCount] = { "EASY", "MEDIUM", "HARD", "DOOR GAME", "ESCAPE" };
            Color colors[btnCount] = { GREEN, YELLOW, RED, ORANGE, PURPLE };

            for (int i = 0; i < btnCount; i++) {
                DrawRectangleRec(difficultyButtons[i], DARKGRAY);

                int tx = difficultyButtons[i].x + (btnWidth / 2 - MeasureText(names[i], 40) / 2);
                int ty = difficultyButtons[i].y + 10;

                DrawText(names[i], tx, ty, 40, colors[i]);
            }

            EndDrawing();
        } break;

        case STATE_EXIT:
            CloseWindow();
            return 0;
        }
    }

    CloseWindow();
    return 0;
}
