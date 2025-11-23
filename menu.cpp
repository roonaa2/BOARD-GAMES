#include <iostream>
#include <limits>
#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "InfinityTic-Tac-Toe.h"

using namespace std;

void displayMenu() {
    cout << "\n>_< Tic Tac Toe Menu >_<\n";
    cout << "1. Classic Tic-Tac-Toe\n";
    cout << "2. Infinity Tic-Tac-Toe\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

void runPolymorphicGame(int choice) {
    // POLYMORPHISM: Base class pointers holding derived objects
    UI<char>* game_ui = nullptr;
    Board<char>* game_board = nullptr;
    
    switch (choice) {
        case 1:
            game_ui = new XO_UI();
            game_board = new X_O_Board();
            break;
        case 2:
            game_ui = new InfinityTicTacToe_UI();
            game_board = new InfinityBoard();
            break;
        default:
            return;
    }
    
    // POLYMORPHIC SETUP: Same code works for all games
    Player<char>** players = game_ui->setup_players();
    GameManager<char> game_manager(game_board, players, game_ui);
    
    // POLYMORPHIC EXECUTION: GameManager calls virtual methods
    game_manager.run();
    
    // Cleanup
    delete game_board;
    delete game_ui;
    for (int i = 0; i < 2; ++i) delete players[i];
    delete[] players;
}

int main() {
    int choice;
    do {
        displayMenu();
        cin >> choice;
        if (choice != 0) {
            runPolymorphicGame(choice);
        }
    } while (choice != 0);
    return 0;
}