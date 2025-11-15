#include "raylib.h"
#include "help.h"

void ShowHelp() {
    bool running = true;
    const int fontSizeTitle = 40;
    const int fontSizeText = 24;

    while (running && !WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(DARKGRAY);

        // Get current screen size
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        // ----- TITLE -----
        const char* title = "HELP MENU";
        int titleWidth = MeasureText(title, fontSizeTitle);
        DrawText(title, screenWidth / 2 - titleWidth / 2, 40, fontSizeTitle, YELLOW);

        // Line under the title
        DrawLine(screenWidth / 2 - 200, 85, screenWidth / 2 + 200, 85, YELLOW);

        // ----- SECTION: GAME OBJECTIVE -----
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

        // ----- SECTION: CONTROLS -----
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

        // Draw each control line centered
        for (int i = 0; i < 5; i++) {
            int textWidth = MeasureText(controls[i], fontSizeText);
            DrawText(controls[i],
                screenWidth / 2 - textWidth / 2,
                260 + i * 30,
                fontSizeText,
                LIGHTGRAY);
        }

        // ----- SECTION: LEVELS -----
        DrawText(section3,
            screenWidth / 2 - MeasureText(section3, 28) / 2,
            440, 28, WHITE);

        const char* levels[] = {
            "Easy   - Small maze with a simple path.",
            "Medium - Maze where the screen briefly flashes black.",
            "Hard   - Only a small circle around you is visible; the rest is dark."
        };

        // Draw each level description
        for (int i = 0; i < 3; i++) {
            int textWidth = MeasureText(levels[i], fontSizeText);
            DrawText(levels[i],
                screenWidth / 2 - textWidth / 2,
                480 + i * 30,
                fontSizeText,
                LIGHTGRAY);
        }

        EndDrawing();

        // Leave help menu on ESC
        if (IsKeyPressed(KEY_ESCAPE))
            running = false;
    }
}
