/**
 * @file ObstaclesTicTacToe.cpp
 * @brief Implementation of the Obstacles Tic-Tac-Toe game classes.
 */
#include "ObstaclesTicTacToe.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <cctype>

using namespace std;

//--------------------------------------- ObstaclesBoard Implementation

ObstaclesBoard::ObstaclesBoard() : Board(6, 6) {
    // Initialize all cells with blank_symbol
    for (auto& row : board) {
        for (auto& cell : row) {
            cell = blank_symbol;
        }
    }
    
    round_counter = 0;
    update_empty_cells();
}

char ObstaclesBoard::get_cell(int x, int y) const {
    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return '\0';
    return board[x][y];
}

void ObstaclesBoard::update_empty_cells() {
    empty_cells.clear();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (board[i][j] == blank_symbol) {
                empty_cells.push_back({i, j});
            }
        }
    }
}

void ObstaclesBoard::add_random_obstacles() {
    update_empty_cells();
    
    // Add up to 2 obstacles if there are empty cells
    int obstacles_to_add = min(2, (int)empty_cells.size());
    
    for (int i = 0; i < obstacles_to_add; i++) {
        if (empty_cells.empty()) break;
        
        // Pick a random empty cell
        int random_index = rand() % empty_cells.size();
        int x = empty_cells[random_index].first;
        int y = empty_cells[random_index].second;
        
        // Place obstacle
        board[x][y] = obstacle_symbol;
        
        // Remove from empty cells list
        empty_cells.erase(empty_cells.begin() + random_index);
        
        cout << "Obstacle placed at position (" << (x+1) << ", " << (y+1) << ")\n";
    }
}

bool ObstaclesBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();
    
    // Validate move coordinates
    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        return false;
    }
    
    // Check if cell is empty
    if (board[x][y] != blank_symbol) {
        return false;
    }
    
    // Apply move
    n_moves++;
    board[x][y] = toupper(mark);
    
    // Check if a complete round finished (both players moved)
    if (n_moves % 2 == 0) {
        round_counter++;
        cout << "\n--- Round " << round_counter << " completed! Adding obstacles... ---\n";
        add_random_obstacles();
    }
    
    return true;
}

bool ObstaclesBoard::check_four_in_row(char symbol) {
    // Check horizontal
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= columns - 4; j++) {
            if (board[i][j] == symbol && 
                board[i][j+1] == symbol && 
                board[i][j+2] == symbol && 
                board[i][j+3] == symbol) {
                return true;
            }
        }
    }
    
    // Check vertical
    for (int i = 0; i <= rows - 4; i++) {
        for (int j = 0; j < columns; j++) {
            if (board[i][j] == symbol && 
                board[i+1][j] == symbol && 
                board[i+2][j] == symbol && 
                board[i+3][j] == symbol) {
                return true;
            }
        }
    }
    
    // Check main diagonal (top-left to bottom-right)
    for (int i = 0; i <= rows - 4; i++) {
        for (int j = 0; j <= columns - 4; j++) {
            if (board[i][j] == symbol && 
                board[i+1][j+1] == symbol && 
                board[i+2][j+2] == symbol && 
                board[i+3][j+3] == symbol) {
                return true;
            }
        }
    }
    
    // Check anti-diagonal (top-right to bottom-left)
    for (int i = 0; i <= rows - 4; i++) {
        for (int j = 3; j < columns; j++) {
            if (board[i][j] == symbol && 
                board[i+1][j-1] == symbol && 
                board[i+2][j-2] == symbol && 
                board[i+3][j-3] == symbol) {
                return true;
            }
        }
    }
    
    return false;
}

bool ObstaclesBoard::is_win(Player<char>* player) {
    return check_four_in_row(player->get_symbol());
}

bool ObstaclesBoard::is_draw(Player<char>* player) {
    // Check if board is full (no empty cells)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (board[i][j] == blank_symbol) {
                return false; // Still has empty cells
            }
        }
    }
    
    // Board is full and no winner
    return !is_win(player);
}

bool ObstaclesBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

int ObstaclesBoard::get_empty_count() const {
    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (board[i][j] == blank_symbol) {
                count++;
            }
        }
    }
    return count;
}

void ObstaclesBoard::set_cell_temp(int x, int y, char value) {
    if (x >= 0 && x < rows && y >= 0 && y < columns) {
        board[x][y] = value;
    }
}

//--------------------------------------- ObstaclesUI Implementation

ObstaclesUI::ObstaclesUI() : UI<char>("Welcome to FCAI Obstacles Tic-Tac-Toe Game", 6) {
    cout << "\nGame Rules:\n";
    cout << "- Board: 6x6 grid\n";
    cout << "- After every round (both players move), 2 random obstacles (#) are added\n";
    cout << "- Obstacles block cells and cannot be used\n";
    cout << "- First to align 4 marks in a row (horizontal, vertical, or diagonal) wins\n";
    cout << "- Game is a draw if board fills without a winner\n\n";
}

Player<char>* ObstaclesUI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";
    
    return new Player<char>(name, symbol, type);
}

Move<char>* ObstaclesUI::get_move(Player<char>* player) {
    int x, y;
    ObstaclesBoard* obs_board = dynamic_cast<ObstaclesBoard*>(player->get_board_ptr());
    
    if (player->get_type() == PlayerType::HUMAN) {
        // Human player input
        bool valid_input = false;
        
        while (!valid_input) {
            cout << "\n" << player->get_name() << "'s turn (" << player->get_symbol() << ")";
            cout << "\nEnter your move (row column) [1-6]: ";
            cin >> x >> y;
            
            // Convert to 0-indexed
            x--;
            y--;
            
            // Validate position
            if (x < 0 || x >= 6 || y < 0 || y >= 6) {
                cout << "Position out of bounds! Use 1-6 for row and column.\n";
                continue;
            }
            
            // Check if cell is empty
            char cell = obs_board->get_cell(x, y);
            if (cell != '.') {
                if (cell == '#') {
                    cout << "That cell has an obstacle! Choose another cell.\n";
                } else {
                    cout << "Cell already occupied!\n";
                }
                continue;
            }
            
            valid_input = true;
        }
    } else {
        // AI player - Smart logic
        cout << "\n" << player->get_name() << " (AI) is thinking...\n";
        
        char ai_symbol = player->get_symbol();
        char opponent_symbol = (ai_symbol == 'X') ? 'O' : 'X';
        
        int best_x = -1, best_y = -1;
        int best_score = -1000;
        
        // Priority 1: Check if AI can win in this move (make 4 in a row)
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                if (obs_board->get_cell(i, j) == '.') {
                    // Try this move
                    obs_board->set_cell_temp(i, j, ai_symbol);
                    if (obs_board->check_four_in_row(ai_symbol)) {
                        best_x = i;
                        best_y = j;
                        obs_board->set_cell_temp(i, j, '.');
                        x = best_x;
                        y = best_y;
                        cout << player->get_name() << " (AI) winning move at (" 
                             << (x+1) << ", " << (y+1) << ")!\n";
                        return new Move<char>(x, y, player->get_symbol());
                    }
                    obs_board->set_cell_temp(i, j, '.');
                }
            }
        }
        
        // Priority 2: Block opponent from winning (if they have 3 in a row)
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                if (obs_board->get_cell(i, j) == '.') {
                    // Try opponent's move
                    obs_board->set_cell_temp(i, j, opponent_symbol);
                    if (obs_board->check_four_in_row(opponent_symbol)) {
                        best_x = i;
                        best_y = j;
                        obs_board->set_cell_temp(i, j, '.');
                        x = best_x;
                        y = best_y;
                        cout << player->get_name() << " (AI) blocking opponent at (" 
                             << (x+1) << ", " << (y+1) << ")!\n";
                        return new Move<char>(x, y, player->get_symbol());
                    }
                    obs_board->set_cell_temp(i, j, '.');
                }
            }
        }
        
        // Priority 3: Look for opportunities to create 3 in a row
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                if (obs_board->get_cell(i, j) == '.') {
                    int score = 0;
                    
                    // Count AI symbols in surrounding positions
                    // Horizontal check
                    int left = max(0, j-3), right = min(5, j+3);
                    for (int k = left; k <= right; k++) {
                        if (obs_board->get_cell(i, k) == ai_symbol) score += 2;
                    }
                    
                    // Vertical check
                    int top = max(0, i-3), bottom = min(5, i+3);
                    for (int k = top; k <= bottom; k++) {
                        if (obs_board->get_cell(k, j) == ai_symbol) score += 2;
                    }
                    
                    // Diagonal checks
                    for (int d = -3; d <= 3; d++) {
                        if (i+d >= 0 && i+d < 6 && j+d >= 0 && j+d < 6) {
                            if (obs_board->get_cell(i+d, j+d) == ai_symbol) score += 2;
                        }
                        if (i+d >= 0 && i+d < 6 && j-d >= 0 && j-d < 6) {
                            if (obs_board->get_cell(i+d, j-d) == ai_symbol) score += 2;
                        }
                    }
                    
                    // Prefer center positions
                    if (i >= 2 && i <= 3 && j >= 2 && j <= 3) score += 3;
                    
                    if (score > best_score) {
                        best_score = score;
                        best_x = i;
                        best_y = j;
                    }
                }
            }
        }
        
        // If we found a good move, use it
        if (best_x != -1 && best_y != -1) {
            x = best_x;
            y = best_y;
        } else {
            // Fallback: find first empty cell
            bool found = false;
            for (int i = 0; i < 6 && !found; i++) {
                for (int j = 0; j < 6 && !found; j++) {
                    if (obs_board->get_cell(i, j) == '.') {
                        x = i;
                        y = j;
                        found = true;
                    }
                }
            }
        }
        
        cout << player->get_name() << " (AI) placed at position (" 
             << (x+1) << ", " << (y+1) << ")!\n";
    }
    
    return new Move<char>(x, y, player->get_symbol());
}