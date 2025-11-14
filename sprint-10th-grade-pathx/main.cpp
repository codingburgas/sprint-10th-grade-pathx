#include "raylib.h"
#include "gameEasy.h"
#include "gameMedium.h"
#include "gameHard.h"
#include "help.h"

enum GameState { STATE_MAIN_MENU, STATE_DIFFICULTY, STATE_EXIT };

int main() {
    InitWindow(800, 600, "Maze Game");
    SetTargetFPS(60);

    GameState state = STATE_MAIN_MENU;

    const float btnWidth = 300.0f;
    const float btnHeight = 60.0f;
    const float gap = 40.0f; // gap between the butons

    while (!WindowShouldClose()) {
        int SW = GetScreenWidth();
        int SH = GetScreenHeight();
        Vector2 mouse = GetMousePosition();

        // --- MAIN MENU BUTTONS ---
        Rectangle btnPlay = { SW / 2.0f - btnWidth / 2.0f, SH / 2.0f - 100.0f, btnWidth, btnHeight };
        Rectangle btnHelp = { SW / 2.0f - btnWidth / 2.0f, SH / 2.0f,       btnWidth, btnHeight };
        Rectangle btnExit = { SW / 2.0f - btnWidth / 2.0f, SH / 2.0f + 100.0f, btnWidth, btnHeight };

        // --- DIFFICULTY BUTTONS ---
        const int btnCount = 3; // easy, medium, hard
        float startY = SH / 2.0f - (btnCount * btnHeight + (btnCount - 1) * gap) / 2.0f;

        Rectangle difficultyButtons[btnCount];
        difficultyButtons[0] = { SW / 2.0f - btnWidth / 2.0f, startY + 0 * (btnHeight + gap), btnWidth, btnHeight }; // Easy
        difficultyButtons[1] = { SW / 2.0f - btnWidth / 2.0f, startY + 1 * (btnHeight + gap), btnWidth, btnHeight }; // Medium
        difficultyButtons[2] = { SW / 2.0f - btnWidth / 2.0f, startY + 2 * (btnHeight + gap), btnWidth, btnHeight }; // Hard

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

            DrawRectangleRec(btnPlay, DARKGRAY);
            DrawText("PLAY", btnPlay.x + 110, btnPlay.y + 10, 40, GREEN);

            DrawRectangleRec(btnHelp, DARKGRAY);
            DrawText("HELP", btnHelp.x + 110, btnHelp.y + 10, 40, YELLOW);

            DrawRectangleRec(btnExit, DARKGRAY);
            DrawText("EXIT", btnExit.x + 110, btnExit.y + 10, 40, RED);

            EndDrawing();
            break;

        case STATE_DIFFICULTY:
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, difficultyButtons[0])) StartEasyGame();
                if (CheckCollisionPointRec(mouse, difficultyButtons[1])) StartMediumGame();
                if (CheckCollisionPointRec(mouse, difficultyButtons[2])) StartHardGame();
            }
            if (IsKeyPressed(KEY_ESCAPE)) state = STATE_MAIN_MENU;

            BeginDrawing();
            ClearBackground(DARKBLUE);

            DrawText("CHOOSE DIFFICULTY", SW / 2 - 330, SH / 2 - 260, 60, WHITE);

            const char* labels[btnCount] = { "EASY", "MEDIUM", "HARD" };
            Color colors[btnCount] = { GREEN, YELLOW, RED };
            for (int i = 0; i < btnCount; ++i) {
                DrawRectangleRec(difficultyButtons[i], DARKGRAY);
                int textX = (int)(difficultyButtons[i].x + (btnWidth / 2.0f - MeasureText(labels[i], 40) / 2.0f));
                int textY = (int)(difficultyButtons[i].y + 10.0f);
                DrawText(labels[i], textX, textY, 40, colors[i]);
            }

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
