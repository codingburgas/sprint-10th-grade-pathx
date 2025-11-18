#include "raylib.h"
#include "help.h"

static int page = 0;

void ShowHelp()
{
    int SW = GetScreenWidth();
    int SH = GetScreenHeight();

    Font font = LoadFont("Orbitron-Regular.ttf");
    SetTextureFilter(font.texture, TEXTURE_FILTER_TRILINEAR);

    Color panel = { 40, 40, 60, 220 };
    Color outline = { 0, 255, 200, 255 };

    int cardWidth = (int)(SW * 0.75f);
    int cardHeight = 400;

    int cx = SW / 2 - cardWidth / 2;
    int cy = SH / 2 - cardHeight / 2;

    Rectangle btnBack;
    btnBack.x = 50.0f;
    btnBack.y = (float)SH - 100.0f;
    btnBack.width = 200.0f;
    btnBack.height = 60.0f;

    Rectangle btnPrev;
    btnPrev.x = 50.0f;
    btnPrev.y = (float)SH / 2.0f - 40.0f;
    btnPrev.width = 60.0f;
    btnPrev.height = 60.0f;

    Rectangle btnNext;
    btnNext.x = (float)SW - 110.0f;
    btnNext.y = (float)SH / 2.0f - 40.0f;
    btnNext.width = 60.0f;
    btnNext.height = 60.0f;

    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (CheckCollisionPointRec(mouse, btnBack))
                break;

            if (CheckCollisionPointRec(mouse, btnPrev))
            {
                page--;
                if (page < 0) page = 2;
            }

            if (CheckCollisionPointRec(mouse, btnNext))
            {
                page++;
                if (page > 2) page = 0;
            }
        }

        BeginDrawing();

        Color bg = { 10, 15, 35, 255 };
        ClearBackground(bg);
        Vector2 titlePos;
        titlePos.x = (float)SW / 2.0f - 240.0f;
        titlePos.y = 40.0f;
        DrawTextEx(font, "HELP MENU", titlePos, 70.0f, 3.0f, WHITE);
        Rectangle cardRect;
        cardRect.x = (float)cx;
        cardRect.y = (float)cy;
        cardRect.width = (float)cardWidth;
        cardRect.height = (float)cardHeight;

        DrawRectangleRounded(cardRect, 0.08f, 20, panel);
        DrawRectangleRoundedLines(cardRect, 0.08f, 20, outline);

        Vector2 pos;
        pos.x = cardRect.x + 40.0f;
        pos.y = cardRect.y + 40.0f;
        if (page == 0)
        {
            // CONTROLS
            DrawTextEx(font, "CONTROLS", pos, 55.0f, 2.0f, YELLOW);
            pos.y += 90.0f;
            DrawTextEx(font, "- Move with ARROW KEYS", pos, 40.0f, 2.0f, WHITE);
            pos.y += 55.0f;
            DrawTextEx(font, "- or with W A S D", pos, 40.0f, 2.0f, WHITE);
            pos.y += 55.0f;
            DrawTextEx(font, "- Press ESC to pause / exit", pos, 40.0f, 2.0f, WHITE);
        }
        else if (page == 1)
        {
            Color greenTitle = { 120, 255, 140, 255 };
            DrawTextEx(font, "RULES", pos, 55.0f, 2.0f, greenTitle);
            pos.y += 90.0f;
            DrawTextEx(font, "- Avoid walls and obstacles", pos, 40.0f, 2.0f, WHITE);
            pos.y += 55.0f;
            DrawTextEx(font, "- Collect coins to level up", pos, 40.0f, 2.0f, WHITE);
            pos.y += 55.0f;
            DrawTextEx(font, "- Every 10 coins = +1 level", pos, 40.0f, 2.0f, WHITE);
            pos.y += 55.0f;
            DrawTextEx(font, "- Have fun!", pos, 40.0f, 2.0f, WHITE);
        }
        else if (page == 2)
        {
            // GAME MODES
            Color skyTitle = { 130, 200, 255, 255 };
            DrawTextEx(font, "GAME MODES", pos, 55.0f, 2.0f, skyTitle);
            pos.y += 90.0f;
            DrawTextEx(font, "EASY    - Simple slow maze.", pos, 40.0f, 2.0f, WHITE);
            pos.y += 50.0f;
            DrawTextEx(font, "MEDIUM  - Screen briefly flashes black.", pos, 40.0f, 2.0f, WHITE);
            pos.y += 50.0f;
            DrawTextEx(font, "HARD    - Only a small circle around player.", pos, 40.0f, 2.0f, WHITE);
            pos.y += 60.0f;
            DrawTextEx(font, "DOOR MAZE - Find keys, then the matching door.", pos, 40.0f, 2.0f, WHITE);
            pos.y += 60.0f;
            DrawTextEx(font, "ESCAPE - 40 seconds to exit the maze.", pos, 40.0f, 2.0f, WHITE);
        }

        Vector2 leftPos;
        leftPos.x = btnPrev.x + 10.0f;
        leftPos.y = btnPrev.y - 5.0f;
        DrawTextEx(font, "<", leftPos, 60.0f, 2.0f, WHITE);

        Vector2 rightPos;
        rightPos.x = btnNext.x + 10.0f;
        rightPos.y = btnNext.y - 5.0f;
        DrawTextEx(font, ">", rightPos, 60.0f, 2.0f, WHITE);
        bool hover = CheckCollisionPointRec(mouse, btnBack);

        Color normalCol = { 0, 140, 220, 255 };
        Color hoverCol = { 0, 180, 255, 255 };
        Color backCol;

        if (hover) backCol = hoverCol;
        else       backCol = normalCol;

        DrawRectangleRounded(btnBack, 0.15f, 8, backCol);
        DrawRectangleRoundedLines(btnBack, 0.15f, 8, WHITE);

        Vector2 backTextPos;
        backTextPos.x = btnBack.x + 60.0f;
        backTextPos.y = btnBack.y + 10.0f;
        DrawTextEx(font, "BACK", backTextPos, 45.0f, 2.0f, WHITE);

        EndDrawing();
    }

    UnloadFont(font);
}