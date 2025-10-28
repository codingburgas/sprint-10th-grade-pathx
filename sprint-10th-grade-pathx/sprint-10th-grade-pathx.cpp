#include "raylib.h"
#include <iostream>
#include <string>
#include <limits>
using namespace std;

// clearing screen
void clearScreen() {
    cout << string(100, '\n');
}

void printTitle() {
    cout << "=== GAME MENU ===\n\n";
}

void StartNewGame() {
    clearScreen();
    cout << "Starting new Game..." << "\n";
    cout << "Press Enter to open Raylib window...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    // Raylib
    InitWindow(800, 600, "Raylib Window - New Game");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("Raylib window is running!", 220, 280, 20, YELLOW);
        EndDrawing();
    }

    CloseWindow();
}

void settingsMenu() {
    clearScreen();
    cout << "[Settings]\n";
    cout << "1. Difficulty (Easy, Medium, Hard)\n";
    cout << "2. Go back\n";
    cout << "\nPress Enter to return...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void helpMenu() {
    clearScreen();
    cout << "[HELP]\n";
    cout << "Use the menu options to navigate the game.\n";
    cout << "Press Enter to return...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void showMenu() {
    clearScreen();
    printTitle();
    cout << "1. Start New Game\n";
    cout << "2. Settings\n";
    cout << "3. Help\n";
    cout << "4. Exit\n";
}

int main() {
    int choice = 0;

    while (true) {
        showMenu();
        cout << "\nEnter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1:
            StartNewGame();
            break;
        case 2:
            settingsMenu();
            break;
        case 3:
            helpMenu();
            break;
        case 4:
            cout << "Exiting game...\n";
            return 0;
        default:
            cout << "Invalid option. Try again.\n";
            break;
        }
    }
}
