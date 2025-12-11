/**
 * @file NumericalTicTacToe.cpp
 * @brief Implementation of the Numerical Tic-Tac-Toe game classes.
 */
#include "NumericalTicTacToe.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

//--------------------------------------- NumericalBoard Implementation

NumericalBoard::NumericalBoard() : Board(3, 3) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
    used_numbers.clear();
}

NumericalBoard::NumericalBoard(const NumericalBoard& other) : Board(3, 3) {
    // Copy board state
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = other.board[i][j];
        }
    }
    used_numbers = other.used_numbers;
    n_moves = other.n_moves;
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

bool NumericalBoard::is_number_used(int number) const {
    for (int num : used_numbers) {
        if (num == number) {
            return true;
        }
    }
    return false;
}

char NumericalBoard::get_cell(int x, int y) const {
    if (x >= 0 && x < rows && y >= 0 && y < columns) {
        return board[x][y];
    }
    return blank_symbol;
}

void NumericalBoard::set_cell_temp(int x, int y, char value) {
    if (x >= 0 && x < rows && y >= 0 && y < columns) {
        board[x][y] = value;
    }
}

int NumericalBoard::evaluate_line(char cells[3]) const {
    int sum = 0;
    int count = 0;
    
    for (int i = 0; i < 3; i++) {
        if (cells[i] != blank_symbol) {
            sum += (cells[i] - '0');
            count++;
        }
    }
    
    // If line is complete and sums to 15, it's a win
    if (count == 3 && sum == 15) {
        return 1000;
    }
    
    // If line has potential to reach 15
    if (count < 3) {
        int remaining = 15 - sum;
        // Check if remaining sum is achievable
        if (remaining > 0 && remaining <= 9) {
            // More weight to lines with 2 numbers already placed
            int score = (count == 2) ? 100 : 10;
            return score - abs(15 - sum);
        }
    }
    
    return 0;
}

vector<int> NumericalBoard::get_available_numbers(bool is_odd) const {
    vector<int> available;
    
    if (is_odd) {
        // Odd numbers: 1, 3, 5, 7, 9
        for (int n = 1; n <= 9; n += 2) {
            if (!is_number_used(n)) {
                available.push_back(n);
            }
        }
    } else {
        // Even numbers: 2, 4, 6, 8
        for (int n = 2; n <= 8; n += 2) {
            if (!is_number_used(n)) {
                available.push_back(n);
            }
        }
    }
    
    return available;
}

vector<pair<int, int>> NumericalBoard::get_empty_positions() const {
    vector<pair<int, int>> empty;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (board[i][j] == blank_symbol) {
                empty.push_back({i, j});
            }
        }
    }
    
    return empty;
}

int NumericalBoard::evaluate_board(char ai_symbol, char opponent_symbol) const {
    int score = 0;
    
    // Check rows
    for (int i = 0; i < 3; i++) {
        char cells[3] = {board[i][0], board[i][1], board[i][2]};
        score += evaluate_line(cells);
    }
    
    // Check columns
    for (int j = 0; j < 3; j++) {
        char cells[3] = {board[0][j], board[1][j], board[2][j]};
        score += evaluate_line(cells);
    }
    
    // Check diagonals
    char diag1[3] = {board[0][0], board[1][1], board[2][2]};
    char diag2[3] = {board[0][2], board[1][1], board[2][0]};
    score += evaluate_line(diag1);
    score += evaluate_line(diag2);
    
    return score;
}

bool NumericalBoard::is_winning_move(int x, int y, char number) const {
    // Create a copy of the board and test the move
    NumericalBoard temp = *this;
    temp.set_cell_temp(x, y, number);
    
    // Create a dummy player for checking win
    Player<char> dummy("dummy", number, PlayerType::HUMAN);
    return temp.is_win(&dummy);
}

bool NumericalBoard::is_blocking_move(int x, int y, char number, char opponent_symbol) const {
    // Test if placing this number would block an opponent's winning move
    NumericalBoard temp = *this;
    temp.set_cell_temp(x, y, number);
    
    // Check all opponent's possible next moves
    vector<int> opp_numbers;
    bool opp_is_odd = (opponent_symbol == 'X');
    
    if (opp_is_odd) {
        for (int n = 1; n <= 9; n += 2) {
            if (!is_number_used(n) && n != (number - '0')) {
                opp_numbers.push_back(n);
            }
        }
    } else {
        for (int n = 2; n <= 8; n += 2) {
            if (!is_number_used(n) && n != (number - '0')) {
                opp_numbers.push_back(n);
            }
        }
    }
    
    auto empty_positions = temp.get_empty_positions();
    for (auto& pos : empty_positions) {
        for (int opp_num : opp_numbers) {
            if (temp.is_winning_move(pos.first, pos.second, '0' + opp_num)) {
                return true; // This move blocks opponent's potential win
            }
        }
    }
    
    return false;
}

//--------------------------------------- NumericalAIPlayer Implementation

NumericalAIPlayer::NumericalAIPlayer(string name, char symbol, AIDifficulty difficulty) 
    : Player<char>(name, symbol, PlayerType::COMPUTER), difficulty(difficulty) {
    opponent_symbol = (symbol == 'X') ? 'O' : 'X';
}

pair<int, tuple<int, int, char>> NumericalAIPlayer::minimax(NumericalBoard& board, int depth, int alpha, int beta, bool is_maximizing) {
    // Create dummy players for evaluation
    Player<char> ai_player("AI", symbol, PlayerType::COMPUTER);
    Player<char> opp_player("Opponent", opponent_symbol, PlayerType::HUMAN);
    
    // Check terminal states
    if (board.is_win(&ai_player)) {
        return {1000 - depth, {-1, -1, '0'}}; // Prefer faster wins
    }
    if (board.is_win(&opp_player)) {
        return {-1000 + depth, {-1, -1, '0'}}; // Prefer slower losses
    }
    if (board.is_draw(&ai_player)) {
        return {0, {-1, -1, '0'}};
    }
    
    if (depth == 0) {
        return {board.evaluate_board(symbol, opponent_symbol), {-1, -1, '0'}};
    }
    
    auto empty_positions = board.get_empty_positions();
    bool is_odd = (symbol == 'X');
    auto available_numbers = board.get_available_numbers(is_odd);
    
    if (is_maximizing) {
        int max_eval = numeric_limits<int>::min();
        tuple<int, int, char> best_move = {-1, -1, '0'};
        
        for (auto& pos : empty_positions) {
            for (int num : available_numbers) {
                // Make move
                NumericalBoard new_board = board;
                new_board.set_cell_temp(pos.first, pos.second, '0' + num);
                
                // Add number to used list
                vector<int> used_nums = new_board.get_used_numbers();
                used_nums.push_back(num);
                new_board.set_used_numbers(used_nums);
                
                // Recursive call
                auto result = minimax(new_board, depth - 1, alpha, beta, false);
                int eval = result.first;
                
                if (eval > max_eval) {
                    max_eval = eval;
                    best_move = {pos.first, pos.second, '0' + num};
                }
                
                alpha = max(alpha, eval);
                if (beta <= alpha) {
                    break; // Beta cut-off
                }
            }
            if (beta <= alpha) {
                break;
            }
        }
        
        return {max_eval, best_move};
    } else {
        int min_eval = numeric_limits<int>::max();
        tuple<int, int, char> best_move = {-1, -1, '0'};
        
        for (auto& pos : empty_positions) {
            bool opp_is_odd = (opponent_symbol == 'X');
            auto opp_numbers = board.get_available_numbers(opp_is_odd);
            
            for (int num : opp_numbers) {
                // Make move
                NumericalBoard new_board = board;
                new_board.set_cell_temp(pos.first, pos.second, '0' + num);
                
                // Add number to used list
                vector<int> used_nums = new_board.get_used_numbers();
                used_nums.push_back(num);
                new_board.set_used_numbers(used_nums);
                
                // Recursive call
                auto result = minimax(new_board, depth - 1, alpha, beta, true);
                int eval = result.first;
                
                if (eval < min_eval) {
                    min_eval = eval;
                    best_move = {pos.first, pos.second, '0' + num};
                }
                
                beta = min(beta, eval);
                if (beta <= alpha) {
                    break; // Alpha cut-off
                }
            }
            if (beta <= alpha) {
                break;
            }
        }
        
        return {min_eval, best_move};
    }
}

tuple<int, int, char> NumericalAIPlayer::get_easy_move(NumericalBoard& board) {
    // Random moves with occasional strategic plays
    auto empty_positions = board.get_empty_positions();
    bool is_odd = (symbol == 'X');
    auto available_numbers = board.get_available_numbers(is_odd);
    
    // Use random device for better randomness
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    
    // 30% chance to make a good move, 70% random
    if (rand() % 100 < 30 && !empty_positions.empty() && !available_numbers.empty()) {
        // Try to make a winning move if available
        for (auto& pos : empty_positions) {
            for (int num : available_numbers) {
                if (board.is_winning_move(pos.first, pos.second, '0' + num)) {
                    return {pos.first, pos.second, '0' + num};
                }
            }
        }
    }
    
    // Random move
    if (!empty_positions.empty() && !available_numbers.empty()) {
        uniform_int_distribution<int> pos_dist(0, empty_positions.size() - 1);
        uniform_int_distribution<int> num_dist(0, available_numbers.size() - 1);
        
        int pos_idx = pos_dist(rng);
        int num_idx = num_dist(rng);
        
        return {empty_positions[pos_idx].first, 
                empty_positions[pos_idx].second, 
                '0' + available_numbers[num_idx]};
    }
    
    return {-1, -1, '0'}; // Should never reach here
}

tuple<int, int, char> NumericalAIPlayer::get_medium_move(NumericalBoard& board) {
    // More strategic AI with some lookahead
    auto empty_positions = board.get_empty_positions();
    bool is_odd = (symbol == 'X');
    auto available_numbers = board.get_available_numbers(is_odd);
    
    // Priority 1: Winning move
    for (auto& pos : empty_positions) {
        for (int num : available_numbers) {
            if (board.is_winning_move(pos.first, pos.second, '0' + num)) {
                return {pos.first, pos.second, '0' + num};
            }
        }
    }
    
    // Priority 2: Block opponent's winning move
    for (auto& pos : empty_positions) {
        for (int num : available_numbers) {
            if (board.is_blocking_move(pos.first, pos.second, '0' + num, opponent_symbol)) {
                return {pos.first, pos.second, '0' + num};
            }
        }
    }
    
    // Priority 3: Take center if available
    if (board.get_cell(1, 1) == '.' && !available_numbers.empty()) {
        // Use the best available number for center
        int best_num = available_numbers[available_numbers.size() / 2];
        return {1, 1, '0' + best_num};
    }
    
    // Priority 4: Random strategic move
    if (!empty_positions.empty() && !available_numbers.empty()) {
        // Prefer corners
        vector<pair<int, int>> corners = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
        for (auto& corner : corners) {
            if (board.get_cell(corner.first, corner.second) == '.') {
                int mid_idx = available_numbers.size() / 2;
                return {corner.first, corner.second, '0' + available_numbers[mid_idx]};
            }
        }
        
        // Random move as fallback
        return get_easy_move(board);
    }
    
    return {-1, -1, '0'};
}

tuple<int, int, char> NumericalAIPlayer::get_hard_move(NumericalBoard& board) {
    // Use minimax with limited depth
    auto result = minimax(board, 3, numeric_limits<int>::min(), numeric_limits<int>::max(), true);
    return result.second;
}

tuple<int, int, char> NumericalAIPlayer::get_best_move(NumericalBoard& board) {
    switch (difficulty) {
        case AIDifficulty::EASY:
            return get_easy_move(board);
        case AIDifficulty::MEDIUM:
            return get_medium_move(board);
        case AIDifficulty::HARD:
            return get_hard_move(board);
        case AIDifficulty::EXPERT:
            return get_hard_move(board); // Expert uses same as hard for now
        default:
            return get_easy_move(board);
    }
}

//--------------------------------------- NumericalUI Implementation

NumericalUI::NumericalUI() : UI<char>("Welcome to FCAI Numerical Tic-Tac-Toe Game", 3) {
    cout << "\nGame Rules:\n";
    cout << "- Player 1 (X) uses odd numbers: 1, 3, 5, 7, 9\n";
    cout << "- Player 2 (O) uses even numbers: 2, 4, 6, 8\n";
    cout << "- Each number can only be used once\n";
    cout << "- First player to make a line (row, column, or diagonal) sum to 15 wins!\n\n";
}

AIDifficulty NumericalUI::select_difficulty() {
    int choice;
    cout << "\nSelect AI Difficulty Level:\n";
    cout << "1. Easy (Random moves)\n";
    cout << "2. Medium (Some strategy)\n";
    cout << "3. Hard (Minimax algorithm)\n";
    cout << "4. Expert (Advanced strategy)\n";
    cout << "Enter choice (1-4): ";
    cin >> choice;
    
    while (choice < 1 || choice > 4) {
        cout << "Invalid choice. Enter 1-4: ";
        cin >> choice;
    }
    
    switch (choice) {
        case 1: return AIDifficulty::EASY;
        case 2: return AIDifficulty::MEDIUM;
        case 3: return AIDifficulty::HARD;
        case 4: return AIDifficulty::EXPERT;
        default: return AIDifficulty::MEDIUM;
    }
}

Player<char>* NumericalUI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::HUMAN) {
        cout << "Creating human player: " << name << " (" << symbol << ")\n";
        if (symbol == 'X') {
            cout << name << " will use odd numbers (1, 3, 5, 7, 9)\n";
        } else {
            cout << name << " will use even numbers (2, 4, 6, 8)\n";
        }
        return new Player<char>(name, symbol, type);
    } else {
        AIDifficulty difficulty = select_difficulty();
        cout << "Creating AI player: " << name << " (" << symbol << ") - ";
        switch (difficulty) {
            case AIDifficulty::EASY: cout << "Easy difficulty\n"; break;
            case AIDifficulty::MEDIUM: cout << "Medium difficulty\n"; break;
            case AIDifficulty::HARD: cout << "Hard difficulty\n"; break;
            case AIDifficulty::EXPERT: cout << "Expert difficulty\n"; break;
        }
        
        if (symbol == 'X') {
            cout << name << " will use odd numbers (1, 3, 5, 7, 9)\n";
        } else {
            cout << name << " will use even numbers (2, 4, 6, 8)\n";
        }
        
        return new NumericalAIPlayer(name, symbol, difficulty);
    }
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
        // AI player logic
        NumericalAIPlayer* ai_player = dynamic_cast<NumericalAIPlayer*>(player);
        if (ai_player) {
            // Get AI's best move
            auto [ai_x, ai_y, ai_number] = ai_player->get_best_move(*num_board);
            x = ai_x;
            y = ai_y;
            number = ai_number - '0';
            
            cout << player->get_name() << " (AI) placed " << number 
                 << " at position (" << (x+1) << ", " << (y+1) << ")\n";
        } else {
            // Fallback to random for basic computer player
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
    }
    
    // Convert number to character for storage
    char number_char = '0' + number;
    
    return new Move<char>(x, y, number_char);
}