#include <iostream>
#include <limits>
#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "InfinityTic-Tac-Toe.h"
#include "Misere.h"
#include "Tic-Tac-Toe_4x4.h"
using namespace std;

void displayMenu() {
    cout << "\n>_< Tic Tac Toe Menu >_<\n";
    cout << "1. SUS game\n";
    cout << "2. Four-in-a-Row\n";
    cout << "3. 5x5 Tic-Tac-Toe\n";
    cout << "4. Word Tic-Tac-Toe\n";
    cout << "5. Misère Tic-Tac-Toe\n";
    cout << "6. Diamond Tic-Tac-Toe\n";
    cout << "7. 4x4 Tic-Tac-Toe\n";
    cout << "8. Pyramid Tic-Tac-Toe\n";
    cout << "9. Numerical Tic-Tac-Toe\n";
    cout << "10. Obstacles Tic-Tac-Toe\n";
    cout << "11. Infinity Tic-Tac-Toe\n";
    cout << "12. Ultimate Tic Tac Toe\n";
    cout << "13. Memory Tic-Tac-Toe\n";
    cout << "0. Exit\n";
 }

void runGame(int choice) {
    //base class pointers
    UI<char>* game_ui = nullptr;
    Board<char>* game_board = nullptr;
    
    switch (choice) {
        case 1: {
            cout << "\n>_< Starting SUS Tic-Tac-Toe >_<\n";
            
            break;
        }
        case 2: {
            cout << "\n>_< Starting Four-in-a-Row Tic-Tac-Toe >_<\n";
          
            break;
        } 
        case 3: {
            cout << "\n>_< Starting 5x5 Tic-Tac-Toe >_<\n";
            
            break;
        }
        case 4: {
            cout << "\n>_< Starting Word Tic-Tac-Toe >_<\n";
            
            break;
        }
        case 5:{
            cout << "\n>_< Starting Misère Tic-Tac-Toe >_<\n"; 
            game_ui = new MisereUI();  //base pointer to derived
            game_board = new MisereBoard();

            break;  
        }
        case 6: {
            cout << "\n>_< Starting Diamond Tic-Tac-Toe >_<\n";             
            break;
        }
        case 7: {
            cout << "\n>_< Starting 4x4 Tic-Tac-Toe >_<\n";     
            game_ui = new X_O_4x4_UI();  //base pointer to derived
            game_board = new X_O_4x4Board();
            break;
        }   
        case 8: {
            cout << "\n>_< Starting Pyramid Tic-Tac-Toe >_<\n";             
            break;
        }     
        case 9: {
            cout << "\n>_< Starting Numerical Tic-Tac-Toe >_<\n";             
            break;
        }
        case 10: {
            cout << "\n>_< Starting Obstacles Tic-Tac-Toe >_<\n";             
            break;                                          
        }   
        case 11:{
            cout << "\n>_< Starting Infinity Tic-Tac-Toe >_<\n";   
            game_ui = new InfinityTicTacToe_UI();  //base pointer to derived
            game_board = new InfinityBoard();
            break;          
        }
        case 12: {
            cout << "\n>_< Starting Ultimate Tic Tac Toe >_<\n";             
            break;      
        }   
        case 13: {      
            cout << "\n>_< Starting Memory Tic-Tac-Toe >_<\n";             
            break;      
    }
    default:
        cout << "Invalid choice!\n";
        return;
}
    // ~ ~ polymorphism ~ ~ 
    Player<char>** players = game_ui->setup_players();
    GameManager<char> game_manager(game_board, players, game_ui);
    
    // GameManager calls virtual methods
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
            runGame(choice);
        }
    } while (choice != 0);
    return 0;
}