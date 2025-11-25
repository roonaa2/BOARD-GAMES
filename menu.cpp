#include <iostream>
#include <limits>
#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "InfinityTic-Tac-Toe.h"
#include "Misere.h"
#include "Tic-Tac-Toe_4x4.h"
#include "NumericalTicTacToe.h"
#include "SUS.h"

using namespace std;

void displayMenu() {
    cout << "\n========================================\n";
    cout << "        TIC TAC TOE GAME MENU\n";
    cout << "========================================\n";
    cout << "1.  SUS game\n";
    cout << "2.  Four-in-a-Row\n";
    cout << "3.  5x5 Tic-Tac-Toe\n";
    cout << "4.  Word Tic-Tac-Toe\n";
    cout << "5.  Misère Tic-Tac-Toe\n";
    cout << "6.  Diamond Tic-Tac-Toe\n";
    cout << "7.  4x4 Tic-Tac-Toe\n";
    cout << "8.  Pyramid Tic-Tac-Toe\n";
    cout << "9.  Numerical Tic-Tac-Toe\n";
    cout << "10. Obstacles Tic-Tac-Toe\n";
    cout << "11. Infinity Tic-Tac-Toe\n";
    cout << "12. Ultimate Tic Tac Toe\n";
    cout << "13. Memory Tic-Tac-Toe\n";
    cout << "0.  Exit\n";
    cout << "========================================\n";
    cout << "Enter your choice (0-13): ";
}

void runGame(int choice) {
    // Base class pointers for polymorphism
    UI<char>* game_ui = nullptr;
    Board<char>* game_board = nullptr;
    
    switch (choice) {
        case 1: {
            cout << "\n>_< Starting SUS Tic-Tac-Toe >_<\n";
            game_ui = new SUS_UI();
            game_board = new SUSBoard();
            break;
        }
        case 2: {
            cout << "\n>_< Starting Four-in-a-Row Tic-Tac-Toe >_<\n";
            cout << "This game is not yet implemented.\n";
            return;
        } 
        case 3: {
            cout << "\n>_< Starting 5x5 Tic-Tac-Toe >_<\n";
            cout << "This game is not yet implemented.\n";
            return;
        }
        case 4: {
            cout << "\n>_< Starting Word Tic-Tac-Toe >_<\n";
            cout << "This game is not yet implemented.\n";
            return;
        }
        case 5: {
            cout << "\n>_< Starting Misère Tic-Tac-Toe >_<\n"; 
            game_ui = new MisereUI();
            game_board = new MisereBoard();
            break;  
        }
        case 6: {
            cout << "\n>_< Starting Diamond Tic-Tac-Toe >_<\n";
            cout << "This game is not yet implemented.\n";
            return;
        }
        case 7: {
            cout << "\n>_< Starting 4x4 Tic-Tac-Toe >_<\n";     
            game_ui = new X_O_4x4_UI();
            game_board = new X_O_4x4Board();
            break;
        }   
        case 8: {
            cout << "\n>_< Starting Pyramid Tic-Tac-Toe >_<\n";
            cout << "This game is not yet implemented.\n";
            return;
        }     
        case 9: {
            cout << "\n>_< Starting Numerical Tic-Tac-Toe >_<\n";
            game_ui = new NumericalUI();
            game_board = new NumericalBoard();
            break;
        }
        case 10: {
            cout << "\n>_< Starting Obstacles Tic-Tac-Toe >_<\n";
            cout << "This game is not yet implemented.\n";
            return;                                          
        }   
        case 11: {
            cout << "\n>_< Starting Infinity Tic-Tac-Toe >_<\n";   
            game_ui = new InfinityTicTacToe_UI();
            game_board = new InfinityBoard();
            break;          
        }
        case 12: {
            cout << "\n>_< Starting Ultimate Tic Tac Toe >_<\n";
            cout << "This game is not yet implemented.\n";
            return;      
        }   
        case 13: {      
            cout << "\n>_< Starting Memory Tic-Tac-Toe >_<\n";
            cout << "This game is not yet implemented.\n";
            return;      
        }
        default:
            cout << "Invalid choice!\n";
            return;
    }
    
    // Setup players using polymorphism
    Player<char>** players = game_ui->setup_players();
    
    // Create game manager with polymorphic objects
    GameManager<char> game_manager(game_board, players, game_ui);
    
    // Run the game using virtual methods
    game_manager.run();
    
    // Cleanup dynamically allocated memory
    delete game_board;
    delete game_ui;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;
}

int main() {
    // Seed random number generator
    srand(static_cast<unsigned int>(time(nullptr)));
    
    int choice;
    
    cout << "\n";
    cout << "========================================\n";
    cout << "   WELCOME TO TIC TAC TOE COLLECTION\n";
    cout << "========================================\n";
    
    do {
        displayMenu();
        
        // Input validation
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        
        if (choice < 0 || choice > 13) {
            cout << "Invalid choice! Please enter a number between 0 and 13.\n";
            continue;
        }
        
        if (choice != 0) {
            runGame(choice);
            
            // Pause before returning to menu
            cout << "\nPress Enter to return to the menu...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        
    } while (choice != 0);
    
    cout << "\n========================================\n";
    cout << "   Thanks for playing! Goodbye!\n";
    cout << "========================================\n\n";
    
    return 0;
}
