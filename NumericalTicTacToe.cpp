/**
 * @file NumericalTicTacToe.cpp
 * @brief Implementation of the Numerical Tic-Tac-Toe game classes.
 */
#include "NumericalTicTacToe.h"
#include <iostream>
#include <iomanip>

using namespace std;

//--------------------------------------- NumericalBoard Implementation

NumericalBoard::NumericalBoard() : Board(3, 3) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
    used_numbers.clear();
}

bool NumericalBoard::update_board(Move<char>* move) {
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
    
    // Apply move
    n_moves++;
    board[x][y] = mark;
    
    // Track used number
    if (isdigit(mark)) {
        used_numbers.push_back(mark - '0');
    }
    
    return true;
}

bool NumericalBoard::is_win(Player<char>* player) {
    // Check all rows, columns, and diagonals for a sum of 15
    
    // Check rows
    for (int i = 0; i < rows; ++i) {
        int sum = 0;
        bool complete = true;
        for (int j = 0; j < columns; ++j) {
            if (board[i][j] == blank_symbol) {
                complete = false;
                break;
            }
            sum += (board[i][j] - '0');
        }
        if (complete && sum == 15) {
            return true;
        }
    }
    
    // Check columns
    for (int j = 0; j < columns; ++j) {
        int sum = 0;
        bool complete = true;
        for (int i = 0; i < rows; ++i) {
            if (board[i][j] == blank_symbol) {
                complete = false;
                break;
            }
            sum += (board[i][j] - '0');
        }
        if (complete && sum == 15) {
            return true;
        }
    }
    
    // Check main diagonal (top-left to bottom-right)
    int sum = 0;
    bool complete = true;
    for (int i = 0; i < rows; ++i) {
        if (board[i][i] == blank_symbol) {
            complete = false;
            break;
        }
        sum += (board[i][i] - '0');
    }
    if (complete && sum == 15) {
        return true;
    }
    
    // Check anti-diagonal (top-right to bottom-left)
    sum = 0;
    complete = true;
    for (int i = 0; i < rows; ++i) {
        if (board[i][columns - i - 1] == blank_symbol) {
            complete = false;
            break;
        }
        sum += (board[i][columns - i - 1] - '0');
    }
    if (complete && sum == 15) {
        return true;
    }
    
    return false;
}

bool NumericalBoard::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool NumericalBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

bool NumericalBoard::is_number_used(int number) {
    for (int num : used_numbers) {
        if (num == number) {
            return true;
        }
    }
    return false;
}

char NumericalBoard::get_cell(int x, int y) const {
    return board[x][y];
}

//--------------------------------------- NumericalUI Implementation

NumericalUI::NumericalUI() : UI<char>("Welcome to FCAI Numerical Tic-Tac-Toe Game", 3) {
    cout << "\nGame Rules:\n";
    cout << "- Player 1 (X) uses odd numbers: 1, 3, 5, 7, 9\n";
    cout << "- Player 2 (O) uses even numbers: 2, 4, 6, 8\n";
    cout << "- Each number can only be used once\n";
    cout << "- First player to make a line (row, column, or diagonal) sum to 15 wins!\n\n";
}

Player<char>* NumericalUI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";
    
    if (symbol == 'X') {
        cout << name << " will use odd numbers (1, 3, 5, 7, 9)\n";
    } else {
        cout << name << " will use even numbers (2, 4, 6, 8)\n";
    }
    
    return new Player<char>(name, symbol, type);
}

bool NumericalUI::is_valid_number(int number, Player<char>* player, NumericalBoard* board) {
    // Check if number is in valid range
    if (number < 1 || number > 9) {
        cout << "Number must be between 1 and 9!\n";
        return false;
    }
    
    // Check if number has already been used
    if (board->is_number_used(number)) {
        cout << "Number " << number << " has already been used!\n";
        return false;
    }
    
    // Check if number matches player's parity
    if (player->get_symbol() == 'X' && number % 2 == 0) {
        cout << "Player X must use odd numbers (1, 3, 5, 7, 9)!\n";
        return false;
    }
    
    if (player->get_symbol() == 'O' && number % 2 != 0) {
        cout << "Player O must use even numbers (2, 4, 6, 8)!\n";
        return false;
    }
    
    return true;
}

Move<char>* NumericalUI::get_move(Player<char>* player) {
    int x, y, number;
    NumericalBoard* num_board = dynamic_cast<NumericalBoard*>(player->get_board_ptr());
    
    if (player->get_type() == PlayerType::HUMAN) {
        // Human player input
        bool valid_input = false;
        
        while (!valid_input) {
            cout << "\n" << player->get_name() << ", enter your move (row column number): ";
            cin >> x >> y >> number;
            
            // Convert to 0-indexed
            x--;
            y--;
            
            // Validate position
            if (x < 0 || x >= 3 || y < 0 || y >= 3) {
                cout << "Position out of bounds! Use 1-3 for row and column.\n";
                continue;
            }
            
            // Check if cell is occupied
            if (num_board->get_cell(x, y) != '.') {
                cout << "Cell already occupied!\n";
                continue;
            }
            
            // Validate number choice
            if (!is_valid_number(number, player, num_board)) {
                continue;
            }
            
            valid_input = true;
        }
    } else {
        // Computer player logic
        
        // Find a valid random move
        bool found = false;
        while (!found) {
            x = rand() % 3;
            y = rand() % 3;
            
            // Check if cell is empty
            if (num_board->get_cell(x, y) == '.') {
                // Generate valid number for this player
                vector<int> available_numbers;
                
                if (player->get_symbol() == 'X') {
                    // Odd numbers
                    for (int n = 1; n <= 9; n += 2) {
                        if (!num_board->is_number_used(n)) {
                            available_numbers.push_back(n);
                        }
                    }
                } else {
                    // Even numbers
                    for (int n = 2; n <= 8; n += 2) {
                        if (!num_board->is_number_used(n)) {
                            available_numbers.push_back(n);
                        }
                    }
                }
                
                if (!available_numbers.empty()) {
                    number = available_numbers[rand() % available_numbers.size()];
                    found = true;
                }
            }
        }
        
        cout << player->get_name() << " (Computer) placed " << number 
             << " at position (" << (x+1) << ", " << (y+1) << ")\n";
    }
    
    // Convert number to character for storage
    char number_char = '0' + number;
    
    return new Move<char>(x, y, number_char);
}