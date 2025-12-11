#include <iostream>
#include <iomanip>
#include <cctype>
#include <climits>
#include "InfinityTic-Tac-Toe.h"
using namespace std;

// ========== InfinityBoard Implementation ==========

InfinityBoard::InfinityBoard() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool InfinityBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol)) {
            if ((n_moves % 3 == 0) && n_moves > 0) {
                remove_oldest_move();
            }
            n_moves++;
            board[x][y] = toupper(mark);
            moves_history.push({x, y});
        
        return true;
    }
    return false;
}

void InfinityBoard::remove_oldest_move() {
    if (moves_history.empty()) return;
    auto oldest = moves_history.front();
    cout << "Removing oldest move at position (" << oldest.first << ", " << oldest.second << ")\n";
    moves_history.pop();
    board[oldest.first][oldest.second] = blank_symbol;
}

bool InfinityBoard::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
    };

    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;
    }

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

bool InfinityBoard::is_draw(Player<char>* player) {
    return (!is_win(player) && n_moves >= 13);
}

bool InfinityBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// ========== InfinityTicTacToe_AI Implementation ==========

InfinityTicTacToe_AI::InfinityTicTacToe_AI(string name, char symbol) 
    : Player<char>(name, symbol, PlayerType::COMPUTER) {}

Move<char>* InfinityTicTacToe_AI::get_move() {
    cout << "\n" << this->name << " is thinking...\n";
    
    // Strategy priority:
    // 1. Win if possible
    Move<char>* move = find_winning_move();
    if (move) {
        cout << this->name << " found a winning move!\n";
        return move;
    }
    
    // 2. Block opponent's winning move
    move = find_blocking_move();
    if (move) {
        cout << this->name << " is blocking opponent!\n";
        return move;
    }
    
    // 3. Make strategic move (create threats)
    move = find_strategic_move();
    if (move) {
        return move;
    }
    
    // 4. Take center or corner
    move = find_center_or_corner();
    if (move) {
        return move;
    }
    
    // 5. Take any available spot
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (is_valid_move(i, j)) {
                return new Move<char>(i, j, this->symbol);
            }
        }
    }
    
    return nullptr;
}

Move<char>* InfinityTicTacToe_AI::find_winning_move() {
    InfinityBoard* board = dynamic_cast<InfinityBoard*>(this->boardPtr);
    
    // Try each empty position and see if it creates a win
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (is_valid_move(i, j)) {
                // Check all winning lines through this position
                // Rows
                if (can_win_in_line(i, 0, i, 1, i, 2, this->symbol))
                    return new Move<char>(i, j, this->symbol);
                // Columns
                if (can_win_in_line(0, j, 1, j, 2, j, this->symbol))
                    return new Move<char>(i, j, this->symbol);
                // Diagonals
                if (i == j && can_win_in_line(0, 0, 1, 1, 2, 2, this->symbol))
                    return new Move<char>(i, j, this->symbol);
                if (i + j == 2 && can_win_in_line(0, 2, 1, 1, 2, 0, this->symbol))
                    return new Move<char>(i, j, this->symbol);
            }
        }
    }
    return nullptr;
}

Move<char>* InfinityTicTacToe_AI::find_blocking_move() {
    char opponent = (this->symbol == 'X') ? 'O' : 'X';
    
    // Try each empty position and see if it blocks opponent's win
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (is_valid_move(i, j)) {
                // Check all winning lines through this position
                // Rows
                if (can_win_in_line(i, 0, i, 1, i, 2, opponent))
                    return new Move<char>(i, j, this->symbol);
                // Columns
                if (can_win_in_line(0, j, 1, j, 2, j, opponent))
                    return new Move<char>(i, j, this->symbol);
                // Diagonals
                if (i == j && can_win_in_line(0, 0, 1, 1, 2, 2, opponent))
                    return new Move<char>(i, j, this->symbol);
                if (i + j == 2 && can_win_in_line(0, 2, 1, 1, 2, 0, opponent))
                    return new Move<char>(i, j, this->symbol);
            }
        }
    }
    return nullptr;
}

Move<char>* InfinityTicTacToe_AI::find_strategic_move() {
    int best_score = -1;
    int best_x = -1, best_y = -1;
    
    // Evaluate each empty position
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (is_valid_move(i, j)) {
                int score = evaluate_position(i, j);
                if (score > best_score) {
                    best_score = score;
                    best_x = i;
                    best_y = j;
                }
            }
        }
    }
    
    if (best_x != -1 && best_y != -1) {
        return new Move<char>(best_x, best_y, this->symbol);
    }
    return nullptr;
}

Move<char>* InfinityTicTacToe_AI::find_center_or_corner() {
    // Try center first (most strategic position)
    if (is_valid_move(1, 1)) {
        return new Move<char>(1, 1, this->symbol);
    }
    
    // Try corners
    int corners[4][2] = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
    for (int i = 0; i < 4; i++) {
        if (is_valid_move(corners[i][0], corners[i][1])) {
            return new Move<char>(corners[i][0], corners[i][1], this->symbol);
        }
    }
    
    return nullptr;
}

int InfinityTicTacToe_AI::evaluate_position(int x, int y) {
    int score = 0;
    
    // Center is most valuable
    if (x == 1 && y == 1) score += 4;
    
    // Corners are next valuable
    if ((x == 0 || x == 2) && (y == 0 || y == 2)) score += 3;
    
    // Count potential two-in-a-rows
    // Check row
    score += count_in_line(x, 0, x, 1, x, 2, this->symbol);
    // Check column
    score += count_in_line(0, y, 1, y, 2, y, this->symbol);
    // Check diagonals if applicable
    if (x == y) score += count_in_line(0, 0, 1, 1, 2, 2, this->symbol);
    if (x + y == 2) score += count_in_line(0, 2, 1, 1, 2, 0, this->symbol);
    
    return score;
}

bool InfinityTicTacToe_AI::can_win_in_line(int x1, int y1, int x2, int y2, int x3, int y3, char symbol) {
    InfinityBoard* board = dynamic_cast<InfinityBoard*>(this->boardPtr);
    
    // Use parent Board class's get_cell() method
    char c1 = board->get_cell(x1, y1);
    char c2 = board->get_cell(x2, y2);
    char c3 = board->get_cell(x3, y3);
    
    // Count symbols and blanks
    int symbol_count = 0;
    int blank_count = 0;
    
    if (c1 == toupper(symbol)) symbol_count++;
    else if (c1 == board->get_blank_symbol()) blank_count++;
    
    if (c2 == toupper(symbol)) symbol_count++;
    else if (c2 == board->get_blank_symbol()) blank_count++;
    
    if (c3 == toupper(symbol)) symbol_count++;
    else if (c3 == board->get_blank_symbol()) blank_count++;
    
    // Can win if we have 2 symbols and 1 blank
    return (symbol_count == 2 && blank_count == 1);
}

bool InfinityTicTacToe_AI::is_valid_move(int x, int y) {
    InfinityBoard* board = dynamic_cast<InfinityBoard*>(this->boardPtr);
    
    // Use parent Board class's get_cell() and InfinityBoard's get_blank_symbol()
    return (x >= 0 && x < 3 && y >= 0 && y < 3 && 
            board->get_cell(x, y) == board->get_blank_symbol());
}

int InfinityTicTacToe_AI::count_in_line(int x1, int y1, int x2, int y2, int x3, int y3, char symbol) {
    InfinityBoard* board = dynamic_cast<InfinityBoard*>(this->boardPtr);
    
    // Use parent Board class's get_cell() method
    char c1 = board->get_cell(x1, y1);
    char c2 = board->get_cell(x2, y2);
    char c3 = board->get_cell(x3, y3);
    
    int count = 0;
    if (c1 == toupper(symbol)) count++;
    if (c2 == toupper(symbol)) count++;
    if (c3 == toupper(symbol)) count++;
    
    // Check if line is still viable (no opponent symbols)
    char opponent = (symbol == 'X') ? 'O' : 'X';
    if (c1 == toupper(opponent) || c2 == toupper(opponent) || c3 == toupper(opponent))
        return 0;
    
    return count;
}

// ========== InfinityTicTacToe_UI Implementation ==========

InfinityTicTacToe_UI::InfinityTicTacToe_UI() : UI<char>("Welcome to Infinity TIC TAC TOE", 3) {
    cout << "\nGame Rules:\n";
    cout << "- Players take turns placing their marks (X or O) on the 3x3 grid.\n";
    cout << "- The objective is to create a line of three of your marks.\n";
    cout << "- A line can be horizontal, vertical, or diagonal.\n";
    cout << "- After every 3 moves, the oldest move on the board is removed to make space for new moves.\n";
    cout << "- The game ends when a player creates a line of three marks or when the board is full without any player winning (draw).\n\n";
}

Player<char>* InfinityTicTacToe_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "AI")
         << " player: " << name << " (" << symbol << ")\n";

    if (type == PlayerType::COMPUTER) {
        return new InfinityTicTacToe_AI(name, symbol);
    }
    return new Player<char>(name, symbol, type);
}

Move<char>* InfinityTicTacToe_UI::get_move(Player<char>* player) {
    int x, y;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
        if (x < 0 || x >= 3 || y < 0 || y >= 3) {
            cout << "Move out of bounds. Try again.\n";
            return get_move(player);
        }
        if (player->get_board_ptr()->get_board_matrix()[x][y] != '.') {
            cout << "Cell already occupied. Try again.\n";
            return get_move(player);
        }
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        // AI player handles its own move generation
        InfinityTicTacToe_AI* ai = dynamic_cast<InfinityTicTacToe_AI*>(player);
        if (ai) {
            return ai->get_move();
        }
        // Fallback to random if cast fails
        x = rand() % 3;
        y = rand() % 3;
        return new Move<char>(x, y, player->get_symbol());
    }
    return new Move<char>(x, y, player->get_symbol());
}