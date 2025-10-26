#include <iostream>
#include <string> 
#include <cmath>
#include <limits>
using namespace std;

const std::string RESET = "\033[0m";
const std::string BOLD = "\033[1m";
const std::string RED = "\033[31m"; //color with strings
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[37m";

void clearScreen() {
}

void StartNewGame() {
    clearScreen();
    cout << GREEN << "Staring new Game" << RESET << "\n";
    cout << "Press Enter to return to main menu";
    cin.ignore(numeric_limits<streamsize>::max(), '\n';
    cin.get();
}

void settingsMenu() {
    clearScreen();
    cout << YELLOW << BOLD << Settings << RESET << "\n\n";
    cout << "1. Difficulty (Easy, Medium, Hard) \n"
}
void helpMenu() {
    clearScreen();
    cout << MAG << BOLD << "HELP" << RESET << "\n\n";
}
void showMenu() {
    clearScreen();
    printTitle();

    cout << CYAN << "Main menu" << RESET << "\n"
}