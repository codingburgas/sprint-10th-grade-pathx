#include "raylib.h"
#include "help.h"

void ShowHelp() {
    bool running = true;

    const int fontTitle = 42;
    const int fontSection = 30;
    const int fontText = 24;

    const Color bgColor{ 30, 30, 30, 255 };

    const char* title = "HELP MENU";
    const char* section1 = "GAME OBJECTIVE";
    const char* section2 = "CONTROLS";
    const char* section3 = "LEVELS";

    const char* objective =
        "Find your way out of the maze using the WASD keys.";

    const char* controls[] = {
        "W - Move up",
        "A - Move left",
        "S - Move down",
        "D - Move right",
        "ESC - Return to main menu"
    };

    const char* levels[] = {
        "Easy       - Small maze with a simple path.",
        "Medium     - Maze where the screen briefly flashes black.",
        "Hard       - Only a small circle around you is visible.",
        "ESCAPE     - You have 35 seconds to escape the maze.",
        "Door Game  - Collect the keys, unlock the doors, win."
    };

    while (running && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(bgColor);

        int screenW = GetScreenWidth();
        int y = 40;

        // TITLE
        int titleWidth = MeasureText(title, fontTitle);
        DrawText(title, screenW / 2 - titleWidth / 2, y, fontTitle, YELLOW);
        y += 60;

        DrawLine(screenW / 2 - 250, y, screenW / 2 + 250, y, YELLOW);
        y += 40;

        // SECTION 1
        int s1w = MeasureText(section1, fontSection);
        DrawText(section1, screenW / 2 - s1w / 2, y, fontSection, WHITE);
        y += 40;

        int objW = MeasureText(objective, fontText);
        DrawText(objective, screenW / 2 - objW / 2, y, fontText, LIGHTGRAY);
        y += 70;

        // SECTION 2
        int s2w = MeasureText(section2, fontSection);
        DrawText(section2, screenW / 2 - s2w / 2, y, fontSection, WHITE);
        y += 50;

        for (int i = 0; i < 5; i++) {
            int w = MeasureText(controls[i], fontText);
            DrawText(controls[i], screenW / 2 - w / 2, y, fontText, LIGHTGRAY);
            y += 30;
        }

        y += 40;

        // SECTION 3
        int s3w = MeasureText(section3, fontSection);
        DrawText(section3, screenW / 2 - s3w / 2, y, fontSection, WHITE);
        y += 50;

        for (int i = 0; i < 5; i++) {
            int w = MeasureText(levels[i], fontText);
            DrawText(levels[i], screenW / 2 - w / 2, y, fontText, LIGHTGRAY);
            y += 28;
        }

        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE)) running = false;
    }
}
