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

    // Load Roboto font from fonts folder
    Font uiFont = LoadFont("Orbitron-Regular.ttf");
    SetTextureFilter(uiFont.texture, TEXTURE_FILTER_TRILINEAR);

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

        Rectangle btnPlay = { float(SW) / 2 - btnWidth / 2, float(SH) / 2 - 100, btnWidth, btnHeight };
        Rectangle btnHelp = { float(SW) / 2 - btnWidth / 2, float(SH) / 2, btnWidth, btnHeight };
        Rectangle btnExit = { float(SW) / 2 - btnWidth / 2, float(SH) / 2 + 100, btnWidth, btnHeight };

        float startY = float(SH) / 2 - (btnCount * btnHeight + (btnCount - 1) * gap) / 2;
        Rectangle difficultyButtons[btnCount] = {
            { float(SW) / 2 - btnWidth / 2, startY + 0 * (btnHeight + gap), btnWidth, btnHeight },
            { float(SW) / 2 - btnWidth / 2, startY + 1 * (btnHeight + gap), btnWidth, btnHeight },
            { float(SW) / 2 - btnWidth / 2, startY + 2 * (btnHeight + gap), btnWidth, btnHeight },
            { float(SW) / 2 - btnWidth / 2, startY + 3 * (btnHeight + gap), btnWidth, btnHeight },
            { float(SW) / 2 - btnWidth / 2, startY + 4 * (btnHeight + gap), btnWidth, btnHeight }
        };

        switch (state) {

        case STATE_MAIN_MENU:
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, btnHelp)) { PlaySound(clickSound); ShowHelp(); }
                if (CheckCollisionPointRec(mouse, btnExit)) { PlaySound(clickSound); state = STATE_EXIT; }
                if (CheckCollisionPointRec(mouse, btnPlay)) { PlaySound(clickSound); state = STATE_DIFFICULTY; }
            }

            BeginDrawing();
            ClearBackground(BLACK);

            // Title
            DrawTextEx(uiFont, "PATH X", { float(SW / 2 - 240), 40 }, 96, 4, GOLD);

            // Total coins
            DrawTextEx(uiFont, TextFormat("TOTAL COINS COLLECTED: %d", totalCoinsCollected),
                { float(SW / 2 - 250), 150 }, 40, 2, YELLOW);

            // Buttons
            DrawRectangleRec(btnPlay, DARKGRAY);
            DrawTextEx(uiFont, "PLAY", { btnPlay.x + 95, btnPlay.y + 12 }, 48, 3, GREEN);

            DrawRectangleRec(btnHelp, DARKGRAY);
            DrawTextEx(uiFont, "HELP", { btnHelp.x + 95, btnHelp.y + 12 }, 48, 3, YELLOW);

            DrawRectangleRec(btnExit, DARKGRAY);
            DrawTextEx(uiFont, "EXIT", { btnExit.x + 95, btnExit.y + 12 }, 48, 3, RED);

            EndDrawing();
            break;

        case STATE_DIFFICULTY:
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, difficultyButtons[0])) { PlaySound(clickSound); StartEasyGame();   totalCoinsCollected += GetEasyGameCoins(); }
                if (CheckCollisionPointRec(mouse, difficultyButtons[1])) { PlaySound(clickSound); StartMediumGame(); totalCoinsCollected += GetMediumGameCoins(); }
                if (CheckCollisionPointRec(mouse, difficultyButtons[2])) { PlaySound(clickSound); StartHardGame();   totalCoinsCollected += GetHardGameCoins(); }
                if (CheckCollisionPointRec(mouse, difficultyButtons[3])) { PlaySound(clickSound); StartGameDoorMaze(); totalCoinsCollected += GetDoorMazeCoins(); }
                if (CheckCollisionPointRec(mouse, difficultyButtons[4])) { PlaySound(clickSound); StartGameSecMaze(); totalCoinsCollected += GetSecMazeCoins(); }
            }

            if (IsKeyPressed(KEY_ESCAPE)) state = STATE_MAIN_MENU;

            BeginDrawing();
            ClearBackground(BLACK);

            DrawTextEx(uiFont, "PATH X", { float(SW / 2 - 240), 40 }, 96, 4, GOLD);
            DrawTextEx(uiFont, "CHOOSE GAME MODE", { float(SW / 2 - 330), 150 }, 60, 3, WHITE);
            DrawTextEx(uiFont, TextFormat("TOTAL COINS: %d", totalCoinsCollected), { float(SW / 2 - 150), 220 }, 36, 2, YELLOW);

            for (int i = 0; i < btnCount; i++) {
                DrawRectangleRec(difficultyButtons[i], GRAY);
                Vector2 textSize = MeasureTextEx(uiFont, difficultyNames[i], 48, 3);
                float tx = difficultyButtons[i].x + btnWidth / 2 - textSize.x / 2;
                DrawTextEx(uiFont, difficultyNames[i], { tx, difficultyButtons[i].y + 8 }, 48, 3, difficultyColors[i]);
            }

            EndDrawing();
            break;

        case STATE_EXIT:
            UnloadSound(clickSound);
            UnloadFont(uiFont);
            CloseAudioDevice();
            CloseWindow();
            return 0;
        }
    }

    UnloadSound(clickSound);
    UnloadFont(uiFont);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
