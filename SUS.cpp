/**
 * @file SUS.cpp
 * @brief Implementation of the SUS Tic-Tac-Toe game classes.
 */
#include "SUS.h"
#include <iostream>
#include <iomanip>

using namespace std;

//--------------------------------------- SUSBoard Implementation

SUSBoard::SUSBoard() : Board(3, 3) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
    player1_score = 0;
    player2_score = 0;
}

char SUSBoard::get_cell(int x, int y) const {
    return board[x][y];
}

int SUSBoard::count_new_sus_sequences(int x, int y) {
    int new_sequences = 0;
    char current = board[x][y];
    
    // Only check if the placed letter is 'S' or 'U'
    if (current != 'S' && current != 'U') {
        return 0;
    }
    
    // Check all possible "S-U-S" patterns that include position (x, y)
    
    // Horizontal patterns
    // Pattern: S at x, U at x+1, S at x+2 (current can be any of these)
    if (y >= 0 && y <= 2) {
        if (board[x][0] == 'S' && board[x][1] == 'U' && board[x][2] == 'S') {
            new_sequences++;
        }
    }
    
    // Vertical patterns
    if (x >= 0 && x <= 2) {
        if (board[0][y] == 'S' && board[1][y] == 'U' && board[2][y] == 'S') {
            new_sequences++;
        }
    }
    
    // Main diagonal (top-left to bottom-right)
    if (x == y) {
        if (board[0][0] == 'S' && board[1][1] == 'U' && board[2][2] == 'S') {
            new_sequences++;
        }
    }
    
    // Anti-diagonal (top-right to bottom-left)
    if (x + y == 2) {
        if (board[0][2] == 'S' && board[1][1] == 'U' && board[2][0] == 'S') {
            new_sequences++;
        }
    }
    
    return new_sequences;
}

bool SUSBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();
    
    // Validate move coordinates
    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        return false;
    }
    
    // Check if cell is already occupied
    if (board[x][y] != blank_symbol) {
        return false;
    }
    
    // Count sequences before placing
    int total_before = player1_score + player2_score;
    
    // Apply move
    n_moves++;
    board[x][y] = mark;
    
    // Count new sequences formed
    int new_sus = count_new_sus_sequences(x, y);
    
    // Update score for the current player
    // Mark is either 'S' or 'U', we need to determine which player
    // Player 1 uses symbol 'X', Player 2 uses symbol 'O'
    // But in this game, both players can place S or U
    // We'll track by alternating turns
    if (n_moves % 2 == 1) {
        // Player 1's turn (odd moves)
        player1_score += new_sus;
    } else {
        // Player 2's turn (even moves)
        player2_score += new_sus;
    }
    
    return true;
}

bool SUSBoard::is_win(Player<char>* player) {
    // Only check for win if game is over (board is full)
    if (n_moves < 9) {
        return false;
    }
    
    // Player wins if they have more SUS sequences than opponent
    if (player->get_symbol() == 'X') {
        return player1_score > player2_score;
    } else {
        return player2_score > player1_score;
    }
}

bool SUSBoard::is_draw(Player<char>* player) {
    // Draw if board is full and scores are equal
    return (n_moves == 9 && player1_score == player2_score);
}

bool SUSBoard::game_is_over(Player<char>* player) {
    return n_moves == 9;
}

int SUSBoard::get_score(Player<char>* player) const {
    if (player->get_symbol() == 'X') {
        return player1_score;
    } else {
        return player2_score;
    }
}

//--------------------------------------- SUS_UI Implementation

SUS_UI::SUS_UI() : UI<char>("Welcome to FCAI SUS Tic-Tac-Toe Game", 3) {
    cout << "\nGame Rules:\n";
    cout << "- Players take turns placing either 'S' or 'U' in empty squares\n";
    cout << "- Each turn, you choose which letter (S or U) to place\n";
    cout << "- Score 1 point for each 'S-U-S' sequence you create\n";
    cout << "- Sequences can be horizontal, vertical, or diagonal\n";
    cout << "- Game ends when the board is full\n";
    cout << "- Player with the most 'S-U-S' sequences wins!\n\n";
}

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";
    
    return new Player<char>(name, symbol, type);
}

Move<char>* SUS_UI::get_move(Player<char>* player) {
    int x, y;
    char letter;
    SUSBoard* sus_board = dynamic_cast<SUSBoard*>(player->get_board_ptr());
    
    if (player->get_type() == PlayerType::HUMAN) {
        // Human player input
        bool valid_input = false;
        
        while (!valid_input) {
            cout << "\n" << player->get_name() << "'s turn";
            cout << " (Current Score: " << sus_board->get_score(player) << ")";
            cout << "\nEnter your move (row column letter[S/U]): ";
            cin >> x >> y >> letter;
            
            // Convert to uppercase
            letter = toupper(letter);
            
            // Convert to 0-indexed
            x--;
            y--;
            
            // Validate position
            if (x < 0 || x >= 3 || y < 0 || y >= 3) {
                cout << "Position out of bounds! Use 1-3 for row and column.\n";
                continue;
            }
            
            // Check if cell is occupied
            if (sus_board->get_cell(x, y) != '.') {
                cout << "Cell already occupied!\n";
                continue;
            }
            
            // Validate letter choice
            if (letter != 'S' && letter != 'U') {
                cout << "Invalid letter! You must choose 'S' or 'U'.\n";
                continue;
            }
            
            valid_input = true;
        }
    } else {
        // Computer player logic - random placement
        bool found = false;
        while (!found) {
            x = rand() % 3;
            y = rand() % 3;
            
            if (sus_board->get_cell(x, y) == '.') {
                // Randomly choose S or U
                letter = (rand() % 2 == 0) ? 'S' : 'U';
                found = true;
            }
        }
        
        cout << player->get_name() << " (Computer) placed '" << letter 
             << "' at position (" << (x+1) << ", " << (y+1) << ")\n";
    }
    
    return new Move<char>(x, y, letter);
}
