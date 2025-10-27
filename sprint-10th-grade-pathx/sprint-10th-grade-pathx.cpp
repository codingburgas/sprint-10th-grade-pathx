#include <iostream>
#include <string>
#include <cmath>
#include <limits>
using namespace std;

// Color constants
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string WHITE = "\033[37m";

// clearing screen
void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void printTitle() {
    cout << BOLD << BLUE << "=== GAME MENU ===" << RESET << "\n\n";
}

// Game start
void StartNewGame() {
    clearScreen();
    cout << GREEN << "Starting new Game.." << RESET << "\n";
    cout << "Press Enter to return to main menu";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void settingsMenu() {
    clearScreen();
    cout << YELLOW << BOLD << "Settings" << RESET << "\n\n";
    cout << "1. Difficulty (Easy, Medium, Hard)\n";
    cout << "2. Go back\n";
    cout << "\nPress Enter to return...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void helpMenu() {
    clearScreen();
    cout << MAGENTA << BOLD << "HELP" << RESET << "\n\n";
    cout << "Use the menu options to navigate the game.\n";
    cout << "Press Enter to return...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void showMenu() {
    clearScreen();
    printTitle();

    cout << CYAN << "Main Menu" << RESET << "\n";
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
            cout << RED << "Exiting game..." << RESET << endl;
            return 0;
        default:
            cout << RED << "Invalid option. Try again." << RESET << endl;
            break;
        }
    }
}