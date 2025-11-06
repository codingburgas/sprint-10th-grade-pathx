#include "raylib.h"
#include <string>

void StartNewGame();
void ShowHelp();
void SettingsMenu();

// Global  colors
static Color BG_COLOR = { 15, 15, 20, 255 };
static Color PANEL_COLOR = { 30, 30, 40, 255 };
static Color TEXT_COLOR = { 220, 220, 255, 255 };
static Color HIGHLIGHT_COLOR = { 130, 180, 255, 255 };

enum MenuOption { MENU_START = 0, MENU_SETTINGS, MENU_HELP, MENU_EXIT, MENU_COUNT };

int main() {
    InitWindow(900, 600, "Maze Game Menu");
    SetTargetFPS(60);

    Font font = LoadFont("resources/RobotoSlab-Bold.ttf");

    int selected = 0;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_UP)) selected = (selected - 1 + MENU_COUNT) % MENU_COUNT;
        if (IsKeyPressed(KEY_DOWN)) selected = (selected + 1) % MENU_COUNT;

        if (IsKeyPressed(KEY_ENTER)) {
            switch (selected) {
            case MENU_START: StartNewGame(); break;
            case MENU_SETTINGS: SettingsMenu(); break;
            case MENU_HELP: ShowHelp(); break;
            case MENU_EXIT: CloseWindow(); return 0;
            }
        }

        BeginDrawing();
        ClearBackground(BG_COLOR);

        DrawTextEx(font, "MAZE PROJECT", { 300, 80 }, 48, 2, TEXT_COLOR);

        const char* options[MENU_COUNT] = {
            "Start New Game",
            "Settings",
            "Help",
            "Exit"
        };

        for (int i = 0; i < MENU_COUNT; i++) {
            Color c = (i == selected) ? HIGHLIGHT_COLOR : TEXT_COLOR;
            DrawTextEx(font, options[i], { 330, (float)(220 + i * 70) }, 32, 2, c);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}


void SettingsMenu() {
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) return;

        BeginDrawing();
        ClearBackground(BG_COLOR);

        DrawText("[Settings]", 50, 50, 40, TEXT_COLOR);
        DrawText("Difficulty: Easy / Medium / Hard (not implemented)", 50, 130, 20, TEXT_COLOR);
        DrawText("Press ESC to return", 50, 200, 20, HIGHLIGHT_COLOR);

        EndDrawing();
    }
}

void ShowHelp() {
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) return;

        BeginDrawing();
        ClearBackground(BG_COLOR);

        DrawText("[Help]", 50, 50, 40, TEXT_COLOR);
        DrawText("Navigate the maze and reach the goal.", 50, 130, 20, TEXT_COLOR);
        DrawText("Use arrow keys to move.", 50, 160, 20, TEXT_COLOR);
        DrawText("Press ESC to return", 50, 220, 20, HIGHLIGHT_COLOR);

        EndDrawing();
    }
}

void StartNewGame() {
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) return;

        BeginDrawing();
        ClearBackground(BG_COLOR);

        DrawText("[Maze Game Placeholder]", 50, 50, 30, TEXT_COLOR);
        DrawText("Game logic is not implemented yet.", 50, 100, 20, TEXT_COLOR);
        DrawText("Press ESC to return", 50, 160, 20, HIGHLIGHT_COLOR);

        EndDrawing();
    }
}
