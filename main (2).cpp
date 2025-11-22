#include <iostream>
#include <limits>
#include <string>

using namespace std;


void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


void displayMenu() {
    cout << "\n>_< Tic Tac Toe Menu >_<\n";
    cout << "1. Pyramid Tic-Tac-Toe\n";
    cout << "2. Four-in-a-Row\n";
    cout << "3. 5x5 Tic-Tac-Toe\n";
    cout << "4. Word Tic-Tac-Toe\n";
    cout << "5. Numerical Tic-Tac-Toe\n";
    cout << "6. Misere Tic-Tac-Toe\n";
    cout << "7. 4x4 Tic-Tac-Toe\n";
    cout << "8. Ultimate Tic-Tac-Toe\n";
    cout << "0. Exit\n";
    cout << "=======================\n";
    cout << "Enter your choice (0-8): ";
}


int getValidChoice() {
    int choice;
    while (true) {
        if (cin >> choice) {
            clearInputBuffer();
            if (choice >= 0 && choice <= 8) {
                return choice;
            }
        } else {
            clearInputBuffer();
        }
        cout << "Invalid choice. Please enter a number between 0 and 8: ";
    }
}


void runGame(int choice) {
    cout << "\n>_< Starting Game " << choice << " >_<\n";
    
    switch (choice) {
        case 1:
            cout << "Pyramid Tic-Tac-Toe would start here!\n";
            break;
        case 2:
            cout << "Four-in-a-Row would start here!\n";
            break;
        case 3:
            cout << "5x5 Tic-Tac-Toe would start here!\n";
            break;
        case 4:
            cout << "Word Tic-Tac-Toe would start here!\n";
            break;
        case 5:
            cout << "Numerical Tic-Tac-Toe would start here!\n";
            break;
        case 6:
            cout << "Misere Tic-Tac-Toe would start here!\n";
            break;
        case 7:
            cout << "4x4 Tic-Tac-Toe would start here!\n";
            break;
        case 8:
            cout << "Ultimate Tic-Tac-Toe would start here!\n";
            break;
    }
    
    cout << "Game " << choice << " completed!\n";
}

int main() {
    cout << "Welcome to Tic Tac Toe Collection!\n";

    int choice;
    do {
        displayMenu();
        choice = getValidChoice();

        if (choice != 0) {
            runGame(choice);
            cout << "\nPress Enter to return to the menu...";
            cin.get();
        }
    } while (choice != 0);

    cout << "\nHope you liked our games >_<!\n";
    return 0;
}