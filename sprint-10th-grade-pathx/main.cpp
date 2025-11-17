#include "raylib.h"
#include "gameEasy.h"
#include "gameMedium.h"
#include "gameHard.h"
#include "help.h"
#include "gameDoorMaze.h"
#include "gameSecMaze.h"

enum GameState { STATE_MAIN_MENU, STATE_DIFFICULTY, STATE_EXIT };

// Global persistent coin counter
static int totalCoinsCollected = 0;

int main() {
    int monitor = GetCurrentMonitor();
    int SW_init = GetMonitorWidth(monitor);
    int SH_init = GetMonitorHeight(monitor);

    InitWindow(SW_init, SH_init, "Maze Game");
    SetWindowState(FLAG_FULLSCREEN_MODE);
    SetTargetFPS(60);

    InitAudioDevice();
    Sound clickSound = LoadSound("click.wav");
    SetSoundVolume(clickSound, 1.0f);

    GameState state = STATE_MAIN_MENU;

    const float btnWidth = 300.0f;
    const float btnHeight = 60.0f;
    const float gap = 60.0f;
    const int btnCount = 5;

    const char* difficultyNames[btnCount] = { "EASY", "MEDIUM", "HARD", "DOOR GAME", "ESCAPE" };
    Color difficultyColors[btnCount] = { GREEN, YELLOW, RED, ORANGE, PURPLE };

    while (!WindowShouldClose()) {
        int SW = GetScreenWidth();
        int SH = GetScreenHeight();
        Vector2 mouse = GetMousePosition();

        // Main menu buttons
        Rectangle btnPlay = { float(SW) / 2.0f - btnWidth / 2, float(SH) / 2 - 100.0f, btnWidth, btnHeight };
        Rectangle btnHelp = { float(SW) / 2.0f - btnWidth / 2, float(SH) / 2.0f, btnWidth, btnHeight };
        Rectangle btnExit = { float(SW) / 2.0f - btnWidth / 2, float(SH) / 2 + 100.0f, btnWidth, btnHeight };

        // Difficulty buttons
        float startY = float(SH) / 2.0f - (btnCount * btnHeight + (btnCount - 1) * gap) / 2.0f;
        Rectangle difficultyButtons[btnCount] = {
            { float(SW) / 2.0f - btnWidth / 2, startY + 0 * (btnHeight + gap), btnWidth, btnHeight },
            { float(SW) / 2.0f - btnWidth / 2, startY + 1 * (btnHeight + gap), btnWidth, btnHeight },
            { float(SW) / 2.0f - btnWidth / 2, startY + 2 * (btnHeight + gap), btnWidth, btnHeight },
            { float(SW) / 2.0f - btnWidth / 2, startY + 3 * (btnHeight + gap), btnWidth, btnHeight },
            { float(SW) / 2.0f - btnWidth / 2, startY + 4 * (btnHeight + gap), btnWidth, btnHeight }
        };

        switch (state) {
        case STATE_MAIN_MENU:
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, btnHelp)) {
                    PlaySound(clickSound);
                    ShowHelp();
                }
                if (CheckCollisionPointRec(mouse, btnExit)) {
                    PlaySound(clickSound);
                    state = STATE_EXIT;
                }
                if (CheckCollisionPointRec(mouse, btnPlay)) {
                    PlaySound(clickSound);
                    state = STATE_DIFFICULTY;
                }
            }

            BeginDrawing();
            ClearBackground(BLACK);

            // Draw title
            DrawText("PATH X", SW / 2 - 200, 50, 80, GOLD);

            // Draw total coins collected in main menu
            DrawText(TextFormat("TOTAL COINS COLLECTED: %d", totalCoinsCollected),
                SW / 2 - 250, 150, 40, YELLOW);

            // Draw buttons
            DrawRectangleRec(btnPlay, DARKGRAY);
            DrawText("PLAY", int(btnPlay.x + 110), int(btnPlay.y + 15), 40, GREEN);
            DrawRectangleRec(btnHelp, DARKGRAY);
            DrawText("HELP", int(btnHelp.x + 110), int(btnHelp.y + 15), 40, YELLOW);
            DrawRectangleRec(btnExit, DARKGRAY);
            DrawText("EXIT", int(btnExit.x + 110), int(btnExit.y + 15), 40, RED);

            EndDrawing();
            break;

        case STATE_DIFFICULTY:
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, difficultyButtons[0])) {
                    PlaySound(clickSound);
                    StartEasyGame();
                    totalCoinsCollected += GetEasyGameCoins();
                }
                if (CheckCollisionPointRec(mouse, difficultyButtons[1])) {
                    PlaySound(clickSound);
                    StartMediumGame();
                    totalCoinsCollected += GetMediumGameCoins();
                }
                if (CheckCollisionPointRec(mouse, difficultyButtons[2])) {
                    PlaySound(clickSound);
                    StartHardGame();
                    totalCoinsCollected += GetHardGameCoins();
                }
                if (CheckCollisionPointRec(mouse, difficultyButtons[3])) {
                    PlaySound(clickSound);
                    StartGameDoorMaze();
                    totalCoinsCollected += GetDoorMazeCoins();
                }
                if (CheckCollisionPointRec(mouse, difficultyButtons[4])) {
                    PlaySound(clickSound);
                    StartGameSecMaze();
                    totalCoinsCollected += GetSecMazeCoins();
                }
            }

            if (IsKeyPressed(KEY_ESCAPE)) state = STATE_MAIN_MENU;

            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("PATH X", SW / 2 - 200, 50, 80, GOLD);
            DrawText("CHOOSE GAME MODE", SW / 2 - 330, 150, 60, WHITE);

            // Show total coins in difficulty selection screen too
            DrawText(TextFormat("TOTAL COINS: %d", totalCoinsCollected),
                SW / 2 - 150, 220, 30, YELLOW);

            for (int i = 0; i < btnCount; i++) {
                DrawRectangleRec(difficultyButtons[i], GRAY);
                int tx = int(difficultyButtons[i].x + (btnWidth / 2 - MeasureText(difficultyNames[i], 40) / 2));
                int ty = int(difficultyButtons[i].y + 10);
                DrawText(difficultyNames[i], tx, ty, 40, difficultyColors[i]);
            }
            EndDrawing();
            break;

        case STATE_EXIT:
            UnloadSound(clickSound);
            CloseAudioDevice();
            CloseWindow();
            return 0;
        }
    }

    UnloadSound(clickSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
