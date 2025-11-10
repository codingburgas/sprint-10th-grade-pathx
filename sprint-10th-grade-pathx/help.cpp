#include <iostream>
#include <limits>
#include "help.h"
using namespace std;

void showHelp() {
    cout << "[HELP]\n";
    cout << "Use the menu options to navigate the game.\n";
    cout << "1. Start a new game to open Raylib window.\n";
    cout << "2. Change settings if available.\n";
    cout << "3. Exit to close the program.\n\n";
    cout << "Press Enter to return...";
    cin.get();
}
