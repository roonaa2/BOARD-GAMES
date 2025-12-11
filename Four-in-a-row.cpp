#include <iostream>
#include <iomanip>
#include <cctype>
#include <climits>
#include "Four-in-a-row.h"
using namespace std;


FourInARowBoard::FourInARowBoard() : Board(6, 7) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

int FourInARowBoard::find_lowest_row(int column) {
    // Find the lowest available row in the column (gravity effect)
    for (int row = rows - 1; row >= 0; row--) {
        if (board[row][column] == blank_symbol) {
            return row;
        }
    }
    return -1; // Column is full
}

bool FourInARowBoard::update_board(Move<char>* move) {
    int col = move->get_y();
    char mark = move->get_symbol();
    
    // Validate column
    if (col < 0 || col >= columns) {
        return false;
    }
    
    // Find lowest available row in this column
    int row = find_lowest_row(col);
    
    if (row == -1) {
        // Column is full
        return false;
    }
    
    // Place the move
    board[row][col] = toupper(mark);
    n_moves++;
    return true;
}

bool FourInARowBoard::is_win(Player<char>* player) {
    char sym = toupper(player->get_symbol());
    
    // Check all positions for a winning line of 4
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            if (board[row][col] != sym) continue;
            
            // Check horizontal (right)
            if (col <= columns - 4) {
                if (board[row][col] == sym && 
                    board[row][col+1] == sym && 
                    board[row][col+2] == sym && 
                    board[row][col+3] == sym) {
                    return true;
                }
            }
            
            // Check vertical (down)
            if (row <= rows - 4) {
                if (board[row][col] == sym && 
                    board[row+1][col] == sym && 
                    board[row+2][col] == sym && 
                    board[row+3][col] == sym) {
                    return true;
                }
            }
            
            // Check diagonal (down-right)
            if (row <= rows - 4 && col <= columns - 4) {
                if (board[row][col] == sym && 
                    board[row+1][col+1] == sym && 
                    board[row+2][col+2] == sym && 
                    board[row+3][col+3] == sym) {
                    return true;
                }
            }
            
            // Check diagonal (down-left)
            if (row <= rows - 4 && col >= 3) {
                if (board[row][col] == sym && 
                    board[row+1][col-1] == sym && 
                    board[row+2][col-2] == sym && 
                    board[row+3][col-3] == sym) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool FourInARowBoard::is_draw(Player<char>* player) {
    // Draw if board is full and no winner
    if (n_moves >= rows * columns && !is_win(player)) {
        return true;
    }
    return false;
}

bool FourInARowBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}


FourInARow_AI::FourInARow_AI(string name, char symbol) 
    : Player<char>(name, symbol, PlayerType::COMPUTER) {}

Move<char>* FourInARow_AI::get_move() {
    cout << "\n" << this->name << " is thinking...\n";
    
    // Strategy priority:
    // 1. Win immediately if possible
    Move<char>* move = find_winning_move();
    if (move) {
        cout << this->name << " found a winning move at column " << move->get_y() << "!\n";
        return move;
    }
    
    // 2. Block opponent's winning move
    move = find_blocking_move();
    if (move) {
        cout << this->name << " is blocking opponent at column " << move->get_y() << "!\n";
        return move;
    }
    
    // 3. Make strategic move
    move = find_strategic_move();
    if (move) {
        return move;
    }
    
    // 4. Play center column if available
    move = find_center_column();
    if (move) {
        return move;
    }
    
    // 5. Play any valid column
    FourInARowBoard* board = dynamic_cast<FourInARowBoard*>(this->boardPtr);
    for (int col = 0; col < 7; col++) {
        if (is_valid_column(col)) {
            return new Move<char>(0, col, this->symbol);
        }
    }
    
    return nullptr;
}

Move<char>* FourInARow_AI::find_winning_move() {
    FourInARowBoard* board = dynamic_cast<FourInARowBoard*>(this->boardPtr);
    
    // Try each column
    for (int col = 0; col < 7; col++) {
        if (!is_valid_column(col)) continue;
        
        int row = board->find_lowest_row(col);
        if (row != -1 && can_win_with_move(row, col, this->symbol)) {
            return new Move<char>(row, col, this->symbol);
        }
    }
    return nullptr;
}

Move<char>* FourInARow_AI::find_blocking_move() {
    FourInARowBoard* board = dynamic_cast<FourInARowBoard*>(this->boardPtr);
    char opponent = (this->symbol == 'X') ? 'O' : 'X';
    
    // Try each column to see if opponent can win
    for (int col = 0; col < 7; col++) {
        if (!is_valid_column(col)) continue;
        
        int row = board->find_lowest_row(col);
        if (row != -1 && can_win_with_move(row, col, opponent)) {
            return new Move<char>(row, col, this->symbol);
        }
    }
    return nullptr;
}

Move<char>* FourInARow_AI::find_strategic_move() {
    FourInARowBoard* board = dynamic_cast<FourInARowBoard*>(this->boardPtr);
    int best_score = -1000;
    int best_col = -1;
    
    // Evaluate each column
    for (int col = 0; col < 7; col++) {
        if (!is_valid_column(col)) continue;
        
        int score = evaluate_column(col);
        if (score > best_score) {
            best_score = score;
            best_col = col;
        }
    }
    
    if (best_col != -1) {
        return new Move<char>(0, best_col, this->symbol);
    }
    return nullptr;
}

Move<char>* FourInARow_AI::find_center_column() {
    // Center column (column 3) is most strategic
    if (is_valid_column(3)) {
        return new Move<char>(0, 3, this->symbol);
    }
    
    // Try columns near center
    for (int offset = 1; offset <= 3; offset++) {
        if (is_valid_column(3 + offset)) {
            return new Move<char>(0, 3 + offset, this->symbol);
        }
        if (is_valid_column(3 - offset)) {
            return new Move<char>(0, 3 - offset, this->symbol);
        }
    }
    return nullptr;
}

int FourInARow_AI::evaluate_column(int col) {
    FourInARowBoard* board = dynamic_cast<FourInARowBoard*>(this->boardPtr);
    int row = board->find_lowest_row(col);
    
    if (row == -1) return -1000; // Column full
    
    int score = 0;
    
    // Center columns are more valuable
    int distance_from_center = abs(col - 3);
    score += (3 - distance_from_center) * 2;
    
    // Evaluate position potential
    score += evaluate_position(row, col);
    
    return score;
}

int FourInARow_AI::evaluate_position(int row, int col) {
    FourInARowBoard* board = dynamic_cast<FourInARowBoard*>(this->boardPtr);
    int score = 0;
    char sym = toupper(this->symbol);
    char opponent = (this->symbol == 'X') ? 'O' : 'X';
    
    // Check all four directions: horizontal, vertical, and both diagonals
    // For each direction, count consecutive pieces
    
    int h_count = count_direction(row, col, 0, 1, sym) + 
                  count_direction(row, col, 0, -1, sym) + 1;
    if (h_count >= 3) score += h_count * 5;
    else if (h_count == 2) score += 2;
    
    int v_count = count_direction(row, col, 1, 0, sym) + 
                  count_direction(row, col, -1, 0, sym) + 1;
    if (v_count >= 3) score += v_count * 5;
    else if (v_count == 2) score += 2;
    
    int d1_count = count_direction(row, col, 1, 1, sym) + 
                   count_direction(row, col, -1, -1, sym) + 1;
    if (d1_count >= 3) score += d1_count * 5;
    else if (d1_count == 2) score += 2;
    
    int d2_count = count_direction(row, col, 1, -1, sym) + 
                   count_direction(row, col, -1, 1, sym) + 1;
    if (d2_count >= 3) score += d2_count * 5;
    else if (d2_count == 2) score += 2;
    
    return score;
}

int FourInARow_AI::count_direction(int row, int col, int dr, int dc, char symbol) {
    FourInARowBoard* board = dynamic_cast<FourInARowBoard*>(this->boardPtr);
    char sym = toupper(symbol);
    int count = 0;
    
    int r = row + dr;
    int c = col + dc;
    
    while (r >= 0 && r < 6 && c >= 0 && c < 7) {
        if (board->get_cell(r, c) == sym) {
            count++;
            r += dr;
            c += dc;
        } else {
            break;
        }
    }
    
    return count;
}

bool FourInARow_AI::can_win_with_move(int row, int col, char symbol) {
    FourInARowBoard* board = dynamic_cast<FourInARowBoard*>(this->boardPtr);
    char sym = toupper(symbol);
    
    // Check if placing at (row, col) creates four in a row
    
    int h_left = count_direction(row, col, 0, -1, sym);
    int h_right = count_direction(row, col, 0, 1, sym);
    if (h_left + h_right + 1 >= 4) return true;
    
    int v_up = count_direction(row, col, -1, 0, sym);
    int v_down = count_direction(row, col, 1, 0, sym);
    if (v_up + v_down + 1 >= 4) return true;
    
    int d1_ul = count_direction(row, col, -1, -1, sym);
    int d1_dr = count_direction(row, col, 1, 1, sym);
    if (d1_ul + d1_dr + 1 >= 4) return true;
    
    int d2_ur = count_direction(row, col, -1, 1, sym);
    int d2_dl = count_direction(row, col, 1, -1, sym);
    if (d2_ur + d2_dl + 1 >= 4) return true;
    
    return false;
}

bool FourInARow_AI::is_valid_column(int col) {
    FourInARowBoard* board = dynamic_cast<FourInARowBoard*>(this->boardPtr);
    return (col >= 0 && col < 7 && board->find_lowest_row(col) != -1);
}


FourInARow_UI::FourInARow_UI() : UI<char>("Welcome to FOUR-IN-A-ROW (Connect Four)", 3) {
    cout << "\nGame Rules:\n";
    cout << "- This is a 6x7 grid game (6 rows, 7 columns).\n";
    cout << "- Players take turns choosing a column (0-6).\n";
    cout << "- Your piece drops to the lowest available position in that column (like gravity).\n";
    cout << "- The objective is to get FOUR of your marks in a row.\n";
    cout << "- A winning line can be horizontal, vertical, or diagonal.\n";
    cout << "- The game ends when a player gets four in a row or the board is full (draw).\n\n";
}

Player<char>* FourInARow_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "AI")
         << " player: " << name << " (" << symbol << ")\n";

    if (type == PlayerType::COMPUTER) {
        return new FourInARow_AI(name, symbol);
    }
    return new Player<char>(name, symbol, type);
}

Move<char>* FourInARow_UI::get_move(Player<char>* player) {
    int col;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << ", enter column (0-6): ";
        cin >> col;
        
        if (col < 0 || col >= 7) {
            cout << "Invalid column. Must be between 0 and 6. Try again.\n";
            return get_move(player);
        }
        
        FourInARowBoard* board = dynamic_cast<FourInARowBoard*>(player->get_board_ptr());
        if (board->find_lowest_row(col) == -1) {
            cout << "Column " << col << " is full. Choose another column.\n";
            return get_move(player);
        }
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        // AI player handles its own move generation
        FourInARow_AI* ai = dynamic_cast<FourInARow_AI*>(player);
        if (ai) {
            return ai->get_move();
        }
        // Fallback to random
        col = rand() % 7;
    }
    
    // Note: Row doesn't matter for Move creation since update_board finds the correct row
    return new Move<char>(0, col, player->get_symbol());
}