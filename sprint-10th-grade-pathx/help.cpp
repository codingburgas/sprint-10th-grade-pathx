#include "raylib.h"
#include "help.h"

void ShowHelp() {
    bool running = true;
    const int fontSizeTitle = 40;
    const int fontSizeText = 24;

    while (running && !WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(DARKGRAY);

        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        const char* title = "HELP MENU";
        int titleWidth = MeasureText(title, fontSizeTitle);
        DrawText(title, screenWidth / 2 - titleWidth / 2, 40, fontSizeTitle, YELLOW);

        DrawLine(screenWidth / 2 - 200, 85, screenWidth / 2 + 200, 85, YELLOW);
        const char* section1 = "GAME OBJECTIVE:";
        const char* section2 = "CONTROLS:";
        const char* section3 = "LEVELS:";

        DrawText(section1,
            screenWidth / 2 - MeasureText(section1, 28) / 2,
            120, 28, WHITE);

        const char* objective = "Find your way out of the maze using the WASD keys.";
        DrawText(objective,
            screenWidth / 2 - MeasureText(objective, fontSizeText) / 2,
            160, fontSizeText, LIGHTGRAY);

        DrawText(section2,
            screenWidth / 2 - MeasureText(section2, 28) / 2,
            220, 28, WHITE);

        const char* controls[] = {
            "W - Move up",
            "A - Move left",
            "S - Move down",
            "D - Move right",
            "ESC - Return to main menu"
        };

        for (int i = 0; i < 5; i++) {
            int textWidth = MeasureText(controls[i], fontSizeText);
            DrawText(controls[i],
                screenWidth / 2 - textWidth / 2,
                260 + i * 30,
                fontSizeText,
                LIGHTGRAY);
        }

        DrawText(section3,
            screenWidth / 2 - MeasureText(section3, 28) / 2,
            440, 28, WHITE);

        const char* levels[] = {
            "Easy   - Small maze with a simple path.",
            "Medium - Maze where the screen briefly flashes black.",
            "Hard   - Only a small circle around you is visible; the rest is dark."
            "ESCAPE - You have 35 seconds to escape the maze"
            "Door Game - Collect the keys, unlock the doors, win."
        };

        for (int i = 0; i < 3; i++) {
            int textWidth = MeasureText(levels[i], fontSizeText);
            DrawText(levels[i],
                screenWidth / 2 - textWidth / 2,
                480 + i * 30,
                fontSizeText,
                LIGHTGRAY);
        }

        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE))
            running = false;
    }
}