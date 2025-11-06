#include "raylib.h"
#include <iostream>
#include <string>
#include <limits>
using namespace std;


//defining screen and grid size
#define screenwidth 2200
#define screenheight 1200
#define cellsize 100
#define grid_width (screenwidth / cellsize)
#define grid_height (screenheight / cellsize)



struct Cell {
    bool visited;
    bool topWall, bottomWall, leftWall, rightWall;
};



void InitializeMaze() {
    for (int x = 0; x < grid_width; x++) {
        for (int y = 0; y < grid_height; y++) {
            maze[x][y].visited = false;
            maze[x][y].topWall = true;
            maze[x][y].bottomWall = true;
            maze[x][y].leftWall = true;
            maze[x][y].rightWall = true;
        }
    }
}

void GenerateMaze(int x, int y) {
    maze[x][y].visited = true;
    do {
        int direction[] = { 0, 1, 2, 3 };// up,right,down and left directions
        for (int i = 0; i < 4; i++) {
            int j = GetRandomValue(i, 3);
            int temp = direction[i];
            direction[i] = direction[j];
            direction[j] = temp;
        }
        bool moved = false;
        for (int i = 0; i < 4; i++) {
            int nx = x, ny = y;
            if (direction[i] == 0) { //Up
                ny -= 1;
            }
            else if (direction[i] == 1) { // Right
                nx += 1;
            }
            else if (direction[i] == 2) { // Down
                ny += 1;
            }
            else if (direction[i] == 3) { // Left
                nx -= 1;
            }
        
        if (nx >= 0 && nx < grid_width && ny >= 0 && ny < grid_height && !maze[nx][ny].visited) {
            if (direction[i] == 0) {
                maze[x][y].topWall = false;
                maze[nx][ny].bottomWall = false;
            }
            else if (direction[i] == 1) {
                maze[x][y].rightWall = false;
                maze[nx][ny].leftWall = false;
            }
            else if (direction[i] == 2) {
                maze[x][y].bottomWall = false;
                maze[nx][ny].topWall = false;
            }
            else if (direction[i] == 3) {
                maze[x][y].leftWall = false;
                maze[nx][ny].rightWall = false;
            }
            GenerateMaze(nx, ny); 
            moved = true;
            break;

           }
        }
        if (!moved) {
            break;
        }

    } while (true);
}


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
    cin.get();*/

    // Raylib


    InitWindow(screenwidth, screenheight, "Raylib Window - New Game");
    
    InitializeMaze();
    GenerateMaze(0, 0);// resets starting position to 0, 0.
    SetTargetFPS(10);

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
