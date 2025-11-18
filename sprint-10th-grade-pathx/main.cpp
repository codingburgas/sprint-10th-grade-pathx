#include "raylib.h"
#include "gameEasy.h"
#include "gameMedium.h"
#include "gameHard.h"
#include "help.h"
#include "gameDoorMaze.h"
#include "gameSecMaze.h"

enum GameState { STATE_MAIN_MENU, STATE_DIFFICULTY, STATE_EXIT };

static int totalCoinsCollected = 0;
static int playerLevel = 1;
static int coinsForNextLevel = 10;

static void UpdatePlayerLevel() {
    playerLevel = (totalCoinsCollected / 10) + 1;
    coinsForNextLevel = playerLevel * 10;
}
void DrawGlowButton(Font font, Rectangle r, const char* label, Color base, Vector2 mouse)
{
    bool hover = CheckCollisionPointRec(mouse, r);

    Color bg;
    if (hover) bg = Fade(base, 0.6f);
    else bg = Fade(base, 0.35f);

    DrawRectangleRec(r, bg);

    if (hover)
        DrawRectangleLinesEx(r, 4, base);
    else {
        Color borderline = Fade(base, 0.6f);
        DrawRectangleLinesEx(r, 2, borderline);
    }

    Vector2 textSize = MeasureTextEx(font, label, 42, 2);

    Vector2 textPos; // Centering text 
    textPos.x = r.x + r.width / 2 - textSize.x / 2;
    textPos.y = r.y + r.height / 2 - textSize.y / 2;

    DrawTextEx(font, label, textPos, 42, 2, WHITE);
}

int main()
{
    int monitor = GetCurrentMonitor();
    int SW = GetMonitorWidth(monitor);
    int SH = GetMonitorHeight(monitor);

    InitWindow(SW, SH, "PATH X");
    SetWindowState(FLAG_FULLSCREEN_MODE);
    SetTargetFPS(60);

    Font uiFont = LoadFont("Orbitron-Regular.ttf");
    SetTextureFilter(uiFont.texture, TEXTURE_FILTER_TRILINEAR);

    InitAudioDevice();
    Sound clickSound = LoadSound("click.wav");
    Sound levelUpSound = LoadSound("levelup.wav");

    GameState state = STATE_MAIN_MENU;

    const float btnWidth = 300;
    const float btnHeight = 60;
    const float gap = 60;
    const int btnCount = 5;

    const char* difficultyNames[btnCount] = {
        "EASY", "MEDIUM", "HARD", "DOOR GAME", "ESCAPE"
    };

    Color difficultyColors[btnCount] = {
        GREEN, YELLOW, RED, ORANGE, PURPLE
    };

    while (!WindowShouldClose())
    {
        SW = GetScreenWidth();
        SH = GetScreenHeight();
        Vector2 mouse = GetMousePosition();

        Rectangle btnPlay = { SW / 2 - btnWidth / 2, SH / 2 - 100, btnWidth, btnHeight };
        Rectangle btnHelp = { SW / 2 - btnWidth / 2, SH / 2,       btnWidth, btnHeight };
        Rectangle btnExit = { SW / 2 - btnWidth / 2, SH / 2 + 100, btnWidth, btnHeight };

        float startY = SH / 2 - (btnCount * btnHeight + (btnCount - 1) * gap) / 2;

        Rectangle difficultyButtons[btnCount];
        for (int i = 0; i < btnCount; i++) {
            difficultyButtons[i].x = SW / 2 - btnWidth / 2;
            difficultyButtons[i].y = startY + i * (btnHeight + gap);
            difficultyButtons[i].width = btnWidth;
            difficultyButtons[i].height = btnHeight;
        }

        switch (state)
        {
        case STATE_MAIN_MENU:
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
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
            for (int y = 0; y < SH; y++)
            {
                float t = (float)y / SH;

                Color g;
                g.r = (unsigned char)(5 + 20 * t);
                g.g = (unsigned char)(5 + 40 * t);
                g.b = (unsigned char)(20 + 130 * t);
                g.a = 255;

                DrawLine(0, y, SW, y, g);
            }
            float time = (float)GetTime();
            for (int i = 0; i < 8; i++) {
                int yy = (int)(time * 100 + i * 140) % SH;

                Color neon = { 0, 255, 180, 255 };
                Color neonFade = Fade(neon, 0.25f);
                DrawLine(0, yy, SW, yy, neonFade);
            }
            Vector2 titlePos = { SW / 2 - 240, 40 };
            Color gold = { 255, 215, 0, 255 };
            DrawTextEx(uiFont, "   PATH X", titlePos, 96, 4, gold);
            Vector2 lvlPos = { SW / 2 - 100, 140 };
            DrawTextEx(uiFont, TextFormat("LEVEL: %d", playerLevel), lvlPos, 45, 2, WHITE);

            Vector2 coinsPos = { SW / 2 - 200, 190 };
            DrawTextEx(uiFont, TextFormat("      TOTAL COINS: %d", totalCoinsCollected),
                coinsPos, 35, 2, YELLOW);
            int coinsInLevel = totalCoinsCollected % 10;
            float prog = coinsInLevel / 10.0f;

            int barW = 300;
            int barH = 30;
            int barX = SW / 2 - barW / 2;
            int barY = 240;

            Color barBG = { 15,15,40,255 };
            DrawRectangle(barX, barY, barW, barH, barBG);

            Color barOutline = { 0,255,180,255 };
            DrawRectangleLines(barX, barY, barW, barH, barOutline);

            Color barFill = { 0,255,130,255 };
            DrawRectangle(barX, barY, (int)(barW * prog), barH, barFill);

            Vector2 barTxt = { SW / 2 - 40, barY + 3 };
            DrawTextEx(uiFont, TextFormat("%d / 10", coinsInLevel),
                barTxt, 26, 2, WHITE);
            Color playC = { 0,180,255,255 };
            Color helpC = { 255,165,0,255 };
            Color exitC = { 255,60,60,255 };

            DrawGlowButton(uiFont, btnPlay, "PLAY", playC, mouse);
            DrawGlowButton(uiFont, btnHelp, "HELP", helpC, mouse);
            DrawGlowButton(uiFont, btnExit, "EXIT", exitC, mouse);

            EndDrawing();
            break;
        }         case STATE_DIFFICULTY:
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                int oldLevel = playerLevel;

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

                UpdatePlayerLevel();

                if (playerLevel > oldLevel)
                    PlaySound(levelUpSound);
            }

            if (IsKeyPressed(KEY_ESCAPE))
                state = STATE_MAIN_MENU;

            BeginDrawing();
            Color bg = { 10, 10, 25, 255 };
            ClearBackground(bg);
            // Struct field assignment 
            Vector2 tpos;
            tpos.x = SW / 2 - 240;
            tpos.y = 40;
            DrawTextEx(uiFont, "    PATH X", tpos, 96, 4, GOLD);

            Vector2 mh;
            mh.x = SW / 2 - 330;
            mh.y = 150;
            DrawTextEx(uiFont, "CHOOSE GAME MODE", mh, 60, 3, WHITE);

            Vector2 lp;
            lp.x = SW / 2 - 100;
            lp.y = 220;
            DrawTextEx(uiFont, TextFormat("LEVEL: %d", playerLevel), lp, 40, 3, BLUE);

            Vector2 cp;
            cp.x = SW / 2 - 150;
            cp.y = 270;
            DrawTextEx(uiFont, TextFormat("TOTAL COINS: %d", totalCoinsCollected),
                cp, 36, 2, YELLOW);

            int coinsNext = totalCoinsCollected % 10;

            Vector2 np;
            np.x = SW / 2 - 150;
            np.y = 310;
            DrawTextEx(uiFont, TextFormat("NEXT LEVEL: %d/10", coinsNext),
                np, 28, 2, GREEN);

            for (int i = 0; i < btnCount; i++)
            {
                bool hover = CheckCollisionPointRec(mouse, difficultyButtons[i]);

                Color base = difficultyColors[i];

                // Darken / brighten
                Color dark;
                dark.r = (unsigned char)(base.r * 0.35f);
                dark.g = (unsigned char)(base.g * 0.35f);
                dark.b = (unsigned char)(base.b * 0.35f);
                dark.a = 255;

                Color mid;
                mid.r = (unsigned char)(base.r * 0.55f);
                mid.g = (unsigned char)(base.g * 0.55f);
                mid.b = (unsigned char)(base.b * 0.55f);
                mid.a = 255;

                Color light;
                light.r = (unsigned char)(base.r * 0.8f);
                light.g = (unsigned char)(base.g * 0.8f);
                light.b = (unsigned char)(base.b * 0.8f);
                light.a = 255;

                if (hover)
                {
                    // Color brightening with RGB clamping
                    int nr = mid.r + 40;
                    int ng = mid.g + 40;
                    int nb = mid.b + 40;

                    if (nr > 255) nr = 255;
                    if (ng > 255) ng = 255;
                    if (nb > 255) nb = 255;

                    mid.r = (unsigned char)nr;
                    mid.g = (unsigned char)ng;
                    mid.b = (unsigned char)nb;
                }

                Rectangle r = difficultyButtons[i];
                Rectangle shadow = { r.x, r.y + 6, r.width, r.height };
                DrawRectangleRec(shadow, dark);
                DrawRectangleRec(r, mid);
                Rectangle highlight = { r.x, r.y, r.width, r.height * 0.25f };
                DrawRectangleRec(highlight, light);
                DrawRectangleLinesEx(r, 3, light);
                Vector2 ts = MeasureTextEx(uiFont, difficultyNames[i], 48, 3);
                Vector2 tp;
                tp.x = r.x + r.width / 2 - ts.x / 2;
                tp.y = r.y + r.height / 2 - ts.y / 2;

                DrawTextEx(uiFont, difficultyNames[i], tp, 48, 3, WHITE);
            }


            EndDrawing();
            break;
        }

        case STATE_EXIT:
        {
            UnloadSound(clickSound);
            UnloadSound(levelUpSound);
            UnloadFont(uiFont);
            CloseAudioDevice();
            CloseWindow();
            return 0;
        }
        } // switch end
    } // while end

    UnloadSound(clickSound);
    UnloadSound(levelUpSound);
    UnloadFont(uiFont);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

