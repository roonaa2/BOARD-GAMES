#include <iostream>
#include <iomanip>
#include <cctype>
#include <vector>
#include <limits>
#include <algorithm>
#include "Tic-Tac-Toe_4x4.h"

using namespace std;

// ============================================================================
// X_O_4x4Board Implementation
// ============================================================================

X_O_4x4Board::X_O_4x4Board() : Board(4, 4) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;

    // Initialize starting positions
    board[0][1] = 'X'; // First row
    board[0][3] = 'X';
    board[0][0] = 'O';
    board[0][2] = 'O';

    board[3][0] = 'X'; // Last row
    board[3][2] = 'X';
    board[3][1] = 'O';
    board[3][3] = 'O';
}

bool X_O_4x4Board::update_board(Move<char>* move) {
    int tx = move->get_x();
    int ty = move->get_y();
    char mark = move->get_symbol();

    // Check if move is within bounds
    if (tx < 0 || tx >= rows || ty < 0 || ty >= columns) {
        cout << "Move out of bounds. Try again.\n";
        return false;
    }

    if (board[tx][ty] != blank_symbol) {
        cout << "Cell already occupied. Try again.\n";
        return false;
    }

    // Search for an adjacent piece belonging to the player
    int dirs[4][2] = {
        {1,0},  // down
        {-1,0}, // up
        {0,1},  // right
        {0,-1}  // left
    };

    int old_x = -1, old_y = -1;

    // Look for any adjacent piece to determine if the move is legal
    for (auto &d : dirs) {
        int nx = tx + d[0];
        int ny = ty + d[1];

        if (nx >= 0 && nx < rows && ny >= 0 && ny < columns) {
            if (board[nx][ny] == mark) {
                old_x = nx;
                old_y = ny;
                break;
            }
        }
    }

    if (old_x == -1) {
        cout << "You must move next to one of your own pieces.\n";
        return false;
    }

    // Remove the old piece 
    board[old_x][old_y] = blank_symbol;

    // Place the piece in the new location
    board[tx][ty] = mark;
    n_moves++; // Increment move counter

    return true;
}

bool X_O_4x4Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();
    return check_win_for_symbol(sym);
}

bool X_O_4x4Board::check_win_for_symbol(char sym) {
    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
    };

    // Check rows and columns for 3 in a row
    for (int i = 0; i < rows; ++i) {
        // Check each row for 3 consecutive pieces
        for (int j = 0; j <= columns - 3; ++j) {
            if (all_equal(board[i][j], board[i][j+1], board[i][j+2]) && board[i][j] == sym)
                return true;
        }
        // Check each column for 3 consecutive pieces
        for (int j = 0; j <= rows - 3; ++j) {
            if (all_equal(board[j][i], board[j+1][i], board[j+2][i]) && board[j][i] == sym)
                return true;
        }
    }

    // Check diagonals (all possible 3-in-a-row diagonals on 4x4 board)
    for (int i = 0; i <= rows - 3; ++i) {
        for (int j = 0; j <= columns - 3; ++j) {
            // Top-left to bottom-right diagonal
            if (all_equal(board[i][j], board[i+1][j+1], board[i+2][j+2]) && board[i][j] == sym)
                return true;
            // Top-right to bottom-left diagonal
            if (all_equal(board[i][j+2], board[i+1][j+1], board[i+2][j]) && board[i][j+2] == sym)
                return true;
        }
    }

    return false;
}

bool X_O_4x4Board::is_draw(Player<char>* player) {
    // Check if board is full (no blank cells)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (board[i][j] == blank_symbol)
                return false;
        }
    }
    return true;
}

bool X_O_4x4Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// ============================================================================
// AI Helper Methods
// ============================================================================

/**
 * Gets all valid moves for the given symbol
 * A valid move must:
 * 1. Be to an empty cell
 * 2. Be adjacent (up/down/left/right) to an existing piece of the same symbol
 */
vector<Move<char>*> X_O_4x4Board::get_valid_moves(char symbol) {
    vector<Move<char>*> valid_moves;
    
    int dirs[4][2] = {
        {1,0},  // down
        {-1,0}, // up
        {0,1},  // right
        {0,-1}  // left
    };
    
    // For each empty cell, check if it's adjacent to a piece of the same symbol
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            // Skip if cell is occupied
            if (board[i][j] != blank_symbol)
                continue;
            
            // Check if this empty cell is adjacent to a piece of the same symbol
            bool is_valid = false;
            for (auto &d : dirs) {
                int nx = i + d[0];
                int ny = j + d[1];
                
                if (nx >= 0 && nx < rows && ny >= 0 && ny < columns) {
                    if (board[nx][ny] == symbol) {
                        is_valid = true;
                        break;
                    }
                }
            }
            
            if (is_valid) {
                valid_moves.push_back(new Move<char>(i, j, symbol));
            }
        }
    }
    
    return valid_moves;
}


pair<int, int> X_O_4x4Board::simulate_move(Move<char>* move) {
    int tx = move->get_x();
    int ty = move->get_y();
    char mark = move->get_symbol();
    
    int dirs[4][2] = {
        {1,0},  // down
        {-1,0}, // up
        {0,1},  // right
        {0,-1}  // left
    };
    
    int old_x = -1, old_y = -1;
    
    // Find adjacent piece to move from
    for (auto &d : dirs) {
        int nx = tx + d[0];
        int ny = ty + d[1];
        
        if (nx >= 0 && nx < rows && ny >= 0 && ny < columns) {
            if (board[nx][ny] == mark) {
                old_x = nx;
                old_y = ny;
                break;
            }
        }
    }
    
    if (old_x != -1) {
        // Remove piece from old position
        board[old_x][old_y] = blank_symbol;
        // Place piece in new position
        board[tx][ty] = mark;
    }
    
    return make_pair(old_x, old_y);
}

/**
 * Undoes a simulated move, restoring the board to its previous state
 */
void X_O_4x4Board::undo_move(Move<char>* move, pair<int, int> old_pos) {
    int tx = move->get_x();
    int ty = move->get_y();
    char mark = move->get_symbol();
    
    // Remove piece from new position
    board[tx][ty] = blank_symbol;
    
    // Restore piece to old position
    if (old_pos.first != -1) {
        board[old_pos.first][old_pos.second] = mark;
    }
}

// ============================================================================
// X_O_4x4_UI Implementation
// ============================================================================

X_O_4x4_UI::X_O_4x4_UI() : UI<char>("Welcome to FCAI X-O 4x4 Game", 3) {
    cout << "X-O 4x4 UI initialized.\n";
    cout << "Players will take turns moving their pieces on a 4x4 board.\n";
    cout << "To make a move, enter the coordinates of the cell where you want to place your piece.\n";
    cout << "You can only move to an adjacent cell next to one of your existing pieces.\n";
    cout << "The goal is to align three of your pieces in a row, column, or diagonal.\n";
    cout << "Ready? Enjoy the game!\n";
}

Player<char>* X_O_4x4_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "AI")
         << " player: " << name << " (" << symbol << ")\n";
    
    if (type == PlayerType::COMPUTER) {
        return new X_O_4x4_AI_Player(name, symbol);
    }
    return new Player<char>(name, symbol, type);
}

Move<char>* X_O_4x4_UI::get_move(Player<char>* player) {
    int x, y;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlayer " << player->get_name() << " (" << player->get_symbol() << ")";
        cout << " - Enter your move x and y (0 to 3): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        // For AI players, call their overridden get_move() method
        X_O_4x4_AI_Player* ai_player = dynamic_cast<X_O_4x4_AI_Player*>(player);
        if (ai_player) {
            return ai_player->get_move();
        }
    }
    
    // Fallback for random computer moves
    x = rand() % 4;
    y = rand() % 4;
    return new Move<char>(x, y, player->get_symbol());
}

// ============================================================================
// X_O_4x4_AI_Player Implementation
// ============================================================================

X_O_4x4_AI_Player::X_O_4x4_AI_Player(string name, char symbol) 
    : Player<char>(name, symbol, PlayerType::COMPUTER) {
    cout << "AI Player '" << name << "' with symbol '" << symbol << "' created.\n";
}

/**
 * Main method that finds the best move for the AI
 * Uses minimax algorithm with alpha-beta pruning
 */
Move<char>* X_O_4x4_AI_Player::get_move() {
    cout << "\nAI " << this->name << " (" << this->symbol << ") is thinking...\n";
    
    X_O_4x4Board* game_board = dynamic_cast<X_O_4x4Board*>(this->boardPtr);
    if (!game_board) {
        cout << "Error: Invalid board type!\n";
        return nullptr;
    }
    
    // Get all possible moves
    vector<Move<char>*> possible_moves = game_board->get_valid_moves(this->symbol);
    
    if (possible_moves.empty()) {
        cout << "No valid moves available!\n";
        return nullptr;
    }
    
    // Determine opponent's symbol
    char opponent_symbol = (this->symbol == 'X') ? 'O' : 'X';
    
    Move<char>* best_move = nullptr;
    int best_score = numeric_limits<int>::min();
    
    // Evaluate each possible move
    for (Move<char>* move : possible_moves) {
        // Simulate the move
        pair<int, int> old_pos = game_board->simulate_move(move);
        
        // Calculate the score for this move using minimax
        // We start with depth 0 and as the minimizing player's turn
        int score = minimax(game_board, 0, false, 
                           numeric_limits<int>::min(), 
                           numeric_limits<int>::max(),
                           this->symbol, opponent_symbol);
        
        // Undo the move
        game_board->undo_move(move, old_pos);
        
        // Update best move if this is better
        if (score > best_score) {
            best_score = score;
            if (best_move) delete best_move;
            best_move = new Move<char>(move->get_x(), move->get_y(), this->symbol);
        }
    }
    
    // Clean up temporary moves
    for (Move<char>* move : possible_moves) {
        delete move;
    }
    
    if (best_move) {
        cout << "AI chooses position (" << best_move->get_x() 
             << ", " << best_move->get_y() << ") with score: " << best_score << "\n";
    }
    
    return best_move;
}

int X_O_4x4_AI_Player::minimax(X_O_4x4Board* board, int depth, bool is_maximizing,
                                int alpha, int beta, char maximizer_symbol, 
                                char minimizer_symbol) {
    
    // Check terminal conditions (win, lose, draw, or max depth reached)
    
    // Check if maximizer (AI) has won
    if (board->check_win_for_symbol(maximizer_symbol)) {
        return 100 - depth; // Prefer faster wins
    }
    
    // Check if minimizer (opponent) has won
    if (board->check_win_for_symbol(minimizer_symbol)) {
        return -100 + depth; // Prefer slower losses
    }
    
    // Check for draw (no blank cells left)
    bool is_draw = true;
    vector<vector<char>> board_matrix = board->get_board_matrix();
    for (int i = 0; i < board->get_rows(); ++i) {
        for (int j = 0; j < board->get_columns(); ++j) {
            if (board_matrix[i][j] == board->get_blank_symbol()) {
                is_draw = false;
                break;
            }
        }
        if (!is_draw) break;
    }
    
    if (is_draw) {
        return 0; // Draw is neutral
    }
    
    // If we've reached maximum depth, evaluate the position
    if (depth >= MAX_DEPTH) {
        return evaluate_board(board, maximizer_symbol, minimizer_symbol);
    }
    
    // Maximizing player's turn (AI)
    if (is_maximizing) {
        int max_eval = numeric_limits<int>::min();
        char current_symbol = maximizer_symbol;
        
        vector<Move<char>*> moves = board->get_valid_moves(current_symbol);
        
        for (Move<char>* move : moves) {
            pair<int, int> old_pos = board->simulate_move(move);
            
            int eval = minimax(board, depth + 1, false, alpha, beta, 
                             maximizer_symbol, minimizer_symbol);
            
            board->undo_move(move, old_pos);
            
            max_eval = max(max_eval, eval);
            alpha = max(alpha, eval);
            
            // Alpha-beta pruning: if beta <= alpha, we can prune this branch
            if (beta <= alpha) {
                // Clean up remaining moves
                for (Move<char>* m : moves) delete m;
                return max_eval;
            }
        }
        
        for (Move<char>* move : moves) delete move;
        return max_eval;
    }
    // Minimizing player's turn (opponent)
    else {
        int min_eval = numeric_limits<int>::max();
        char current_symbol = minimizer_symbol;
        
        vector<Move<char>*> moves = board->get_valid_moves(current_symbol);
        
        for (Move<char>* move : moves) {
            pair<int, int> old_pos = board->simulate_move(move);
            
            int eval = minimax(board, depth + 1, true, alpha, beta,
                             maximizer_symbol, minimizer_symbol);
            
            board->undo_move(move, old_pos);
            
            min_eval = min(min_eval, eval);
            beta = min(beta, eval);
            
            // Alpha-beta pruning
            if (beta <= alpha) {
                for (Move<char>* m : moves) delete m;
                return min_eval;
            }
        }
        
        for (Move<char>* move : moves) delete move;
        return min_eval;
    }
}


int X_O_4x4_AI_Player::evaluate_board(X_O_4x4Board* board, 
                                      char maximizer_symbol, 
                                      char minimizer_symbol) {
    int score = 0;
    vector<vector<char>> board_matrix = board->get_board_matrix();
    
    // Count potential winning lines for both players
    // A line with 2 pieces and 1 empty is more valuable
    
    auto count_line = [&](char a, char b, char c) -> int {
        int max_count = 0, min_count = 0, empty_count = 0;
        
        if (a == maximizer_symbol) max_count++;
        else if (a == minimizer_symbol) min_count++;
        else if (a == board->get_blank_symbol()) empty_count++;
        
        if (b == maximizer_symbol) max_count++;
        else if (b == minimizer_symbol) min_count++;
        else if (b == board->get_blank_symbol()) empty_count++;
        
        if (c == maximizer_symbol) max_count++;
        else if (c == minimizer_symbol) min_count++;
        else if (c == board->get_blank_symbol()) empty_count++;
        
        // If line has both symbols, it's blocked
        if (max_count > 0 && min_count > 0) return 0;
        
        // Score based on how many pieces in a row
        if (max_count == 2 && empty_count == 1) return 10;  // Two in a row
        if (max_count == 1 && empty_count == 2) return 1;   // One with potential
        if (min_count == 2 && empty_count == 1) return -10; // Opponent two in a row
        if (min_count == 1 && empty_count == 2) return -1;  // Opponent one with potential
        
        return 0;
    };
    
    // Check all rows
    for (int i = 0; i < board->get_rows(); ++i) {
        for (int j = 0; j <= board->get_columns() - 3; ++j) {
            score += count_line(board_matrix[i][j], 
                              board_matrix[i][j+1], 
                              board_matrix[i][j+2]);
        }
    }
    
    // Check all columns
    for (int i = 0; i <= board->get_rows() - 3; ++i) {
        for (int j = 0; j < board->get_columns(); ++j) {
            score += count_line(board_matrix[i][j], 
                              board_matrix[i+1][j], 
                              board_matrix[i+2][j]);
        }
    }
    
    // Check diagonals
    for (int i = 0; i <= board->get_rows() - 3; ++i) {
        for (int j = 0; j <= board->get_columns() - 3; ++j) {
            // Top-left to bottom-right
            score += count_line(board_matrix[i][j], 
                              board_matrix[i+1][j+1], 
                              board_matrix[i+2][j+2]);
            // Top-right to bottom-left
            score += count_line(board_matrix[i][j+2], 
                              board_matrix[i+1][j+1], 
                              board_matrix[i+2][j]);
        }
    }
    
    return score;
}