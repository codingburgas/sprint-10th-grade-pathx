#include <iostream>
#include <string>
#include <limits>
#include "game.h"
#include "help.h"
using namespace std;


void clearScreen() {
#if defined(_WIN32)
    system("cls");
#else
    system("clear");
#endif
}

void printTitle() {
    cout << "=== GAME MENU ===\n\n";
}

void settingsMenu() {
    clearScreen();
    cout << "[Settings]\n";
    cout << "1. Difficulty (Easy, Medium, Hard)\n";
    cout << "2. Go back\n\n";
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

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            startNewGame();
            break;
        case 2:
            settingsMenu();
            break;
        case 3:
            showHelp();
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
