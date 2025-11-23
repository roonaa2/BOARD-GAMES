/**
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "NumericalTicTacToe.h"

using namespace std;

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getValidChoice() {
    int choice;
    while (true) {
        if (cin >> choice) {
            clearInputBuffer();
            if (choice >= 0 && choice <= 1) {
                return choice;
            }
        }
        else {
            clearInputBuffer();
        }
        cout << "Invalid choice. Please enter 0 or 1: ";
    }
}

void displayMenu() {
    cout << "\n>_< Tic Tac Toe Menu >_<\n";
    cout << "1. Numerical Tic-Tac-Toe\n";
    cout << "0. Exit\n";
    cout << "=======================\n";
    cout << "Enter your choice (0-1): ";
}

void runNumericalGame() {
    cout << "\n>_< Starting Numerical Tic-Tac-Toe >_<\n";
    try {
        int modeSelection;
        cout << "Choose game mode:" << endl;
        cout << "1. Play against another player" << endl;
        cout << "2. Play against the computer" << endl;

        while (!(cin >> modeSelection) || (modeSelection != 1 && modeSelection != 2)) {
            clearInputBuffer();
            cout << "Invalid choice. Please enter 1 or 2: ";
        }
        clearInputBuffer();

        NumericalTicTacToe gameBoard;
        NumericalTicTacToePlayer* playerOne;
        NumericalTicTacToePlayer* playerTwo;

        string player1Name, player2Name;
        cout << "Enter name for Player 1: ";
        getline(cin, player1Name);

        if (modeSelection == 1) {
            cout << "Enter name for Player 2: ";
            getline(cin, player2Name);
            playerOne = new NumericalTicTacToePlayer(player1Name, 1);
            playerTwo = new NumericalTicTacToePlayer(player2Name, 2);
        }
        else {
            playerOne = new NumericalTicTacToePlayer(player1Name, 1);
            playerTwo = new NumericalTicTacToePlayer("Computer", 2);
        }

        NumericalTicTacToeManager gameManager(&gameBoard, playerOne, playerTwo);
        gameManager.run();

        delete playerOne;
        delete playerTwo;
    }
    catch (...) {
        cout << "An error occurred during the game.\n";
    }
}

int main() {
    cout << "Welcome to Numerical Tic Tac Toe!\n";

    int choice;
    do {
        displayMenu();
        choice = getValidChoice();

        if (choice == 1) {
            runNumericalGame();
            cout << "\nPress Enter to return to the menu...";
            clearInputBuffer();
            cin.get();
        }
    } while (choice != 0);

    cout << "\nThanks for playing Numerical Tic Tac Toe!\n";
    return 0;
} 
*/