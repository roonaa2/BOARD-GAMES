#include "Pyramid.h"
#include <iostream>
#include <limits>
using namespace std;

// ==================== BOARD ====================

PyramidBoard::PyramidBoard() : Board(3,5)
{
    valid = vector<vector<bool>>(3, vector<bool>(5, false));

    // top
    valid[0][2] = true;

    // middle
    valid[1][1] = true;
    valid[1][2] = true;
    valid[1][3] = true;

    // bottom
    for(int i=0;i<5;i++)
        valid[2][i] = true;

    for(int i=0;i<3;i++)
        for(int j=0;j<5;j++)
            board[i][j] = valid[i][j] ? blank : ' ';
}

bool PyramidBoard::is_valid_position(int r, int c) const
{
    if(r<0||r>=3||c<0||c>=5) return false;
    return valid[r][c];
}

bool PyramidBoard::is_cell_empty(int r, int c) const
{
    return is_valid_position(r,c) && board[r][c] == blank;
}

bool PyramidBoard::update_board(Move<char>* move)
{
    int r = move->get_x();
    int c = move->get_y();
    char s = move->get_symbol();

    if (!is_cell_empty(r,c)) return false;

    board[r][c] = s;
    n_moves++;
    return true;
}

// Winning condition: 3 in a line (straight lines only)
bool PyramidBoard::is_win(Player<char>* player)
{
    char s = player->get_symbol();

    // Horizontal lines
    if(board[1][1]==s && board[1][2]==s && board[1][3]==s) return true; // Middle row
    if(board[2][0]==s && board[2][1]==s && board[2][2]==s) return true; // Bottom left
    if(board[2][1]==s && board[2][2]==s && board[2][3]==s) return true; // Bottom middle
    if(board[2][2]==s && board[2][3]==s && board[2][4]==s) return true; // Bottom right

    // Vertical lines (straight down only)
    if(board[0][2]==s && board[1][2]==s && board[2][2]==s) return true; // Center vertical

    // Diagonal lines
    if(board[0][2]==s && board[1][1]==s && board[2][0]==s) return true; // Top to bottom-left
    if(board[0][2]==s && board[1][3]==s && board[2][4]==s) return true; // Top to bottom-right

    return false;
}

bool PyramidBoard::is_draw(Player<char>*)
{
    return n_moves == 9;
}

bool PyramidBoard::game_is_over(Player<char>* player)
{
    return is_win(player) || is_draw(player);
}

// ==================== HELPER METHODS FOR AI ====================

vector<pair<int, int>> PyramidBoard::get_available_moves() const
{
    vector<pair<int, int>> moves;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 5; j++) {
            if(is_cell_empty(i, j)) {
                moves.push_back({i, j});
            }
        }
    }
    return moves;
}

char PyramidBoard::get_cell_value(int r, int c) const
{
    if(is_valid_position(r, c)) {
        return board[r][c];
    }
    return ' ';
}

void PyramidBoard::set_cell_value(int r, int c, char value)
{
    if(is_valid_position(r, c)) {
        board[r][c] = value;
    }
}

void PyramidBoard::increment_moves()
{
    n_moves++;
}

void PyramidBoard::decrement_moves()
{
    n_moves--;
}

// ==================== UI ====================

PyramidUI::PyramidUI() : UI(3) {}

Move<char>* PyramidUI::get_move(Player<char>* player)
{
    // Check if it's an AI player
    PyramidAIPlayer* ai = dynamic_cast<PyramidAIPlayer*>(player);
    if(ai) {
        return ai->get_best_move();
    }
    
    // Human player
    int r, c;
    cout << player->get_name() 
         << " (" << player->get_symbol() << ") - Enter row and col: ";
    cin >> r >> c;

    return new Move<char>(r, c, player->get_symbol());
}

Player<char>** PyramidUI::setup_players()
{
    Player<char>** players = new Player<char>*[2];
    string name;
    int choice;

    cout << "=== PYRAMID TIC-TAC-TOE ===\n\n";
    
    // Player 1 (X)
    cout << "Player 1 (X) - Choose type:\n";
    cout << "1. Human\n";
    cout << "2. AI\n";
    cout << "Enter choice: ";
    cin >> choice;
    
    if(choice == 2) {
        players[0] = create_player(name = "AI-X", 'X', PlayerType::AI);
    } else {
        cout << "Enter name: ";
        cin.ignore();
        getline(cin, name);
        players[0] = create_player(name, 'X', PlayerType::HUMAN);
    }

    // Player 2 (O)
    cout << "\nPlayer 2 (O) - Choose type:\n";
    cout << "1. Human\n";
    cout << "2. AI\n";
    cout << "Enter choice: ";
    cin >> choice;
    
    if(choice == 2) {
        players[1] = create_player(name = "AI-O", 'O', PlayerType::AI);
    } else {
        cout << "Enter name: ";
        cin.ignore();
        getline(cin, name);
        players[1] = create_player(name, 'O', PlayerType::HUMAN);
    }

    cout << "\n";
    return players;
}

Player<char>* PyramidUI::create_player(string& name, char symbol, PlayerType type)
{
    if(type == PlayerType::AI) {
        return new PyramidAIPlayer(name, symbol);
    } else {
        return new Player<char>(name, symbol, type);
    }
}

// ==================== AI PLAYER ====================

PyramidAIPlayer::PyramidAIPlayer(string name, char symbol) 
    : Player<char>(name, symbol, PlayerType::AI)
{
}

char PyramidAIPlayer::get_opponent_symbol() const
{
    return (this->symbol == 'X') ? 'O' : 'X';
}


// AI evaluate every board state to take the best move
int PyramidAIPlayer::evaluate()
{
    PyramidBoard* pb = dynamic_cast<PyramidBoard*>(this->boardPtr);
    if(!pb) return 0;

    // Check if AI wins
    if(pb->is_win(this)) {
        return 10;
    }

    // Check if opponent wins
    Player<char> opponent("temp", get_opponent_symbol(), PlayerType::HUMAN);
    opponent.set_board_ptr(pb);
    if(pb->is_win(&opponent)) {
        return -10;
    }

    // Game not over or draw
    return 0;
}

// Minimax algorithm with alpha-beta pruning to find best move using recursion and backtracking , 
//after evaluating each possible move it recursively explores further moves until terminal states are reached.
int PyramidAIPlayer::minimax(bool isMaximizing, int depth, int alpha, int beta)
{
    PyramidBoard* pb = dynamic_cast<PyramidBoard*>(this->boardPtr);
    if(!pb) return 0;

    // Evaluate current state
    int score = evaluate();

    // Terminal states (game over)
    if(score == 10) return score - depth;
    if(score == -10) return score + depth;
    if(pb->is_draw(this)) return 0;

    if(isMaximizing) {
        // AI's turn - maximize score
        int maxEval = numeric_limits<int>::min();
        vector<pair<int, int>> moves = pb->get_available_moves();

        for(auto& move : moves) {
            int r = move.first;
            int c = move.second;

            // Save old value and make move
            char oldVal = pb->get_cell_value(r, c);
            pb->set_cell_value(r, c, this->symbol);
            pb->increment_moves();

            // Recursive call
            int eval = minimax(false, depth + 1, alpha, beta);

            // Undo move
            pb->set_cell_value(r, c, oldVal);
            pb->decrement_moves();

            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);

            // Alpha-beta pruning
            if(beta <= alpha) break;
        }
        return maxEval;
    }
    else {
        // Opponent's turn - minimize score
        int minEval = numeric_limits<int>::max();
        vector<pair<int, int>> moves = pb->get_available_moves();

        for(auto& move : moves) {
            int r = move.first;
            int c = move.second;

            // Save old value and make move
            char oldVal = pb->get_cell_value(r, c);
            pb->set_cell_value(r, c, get_opponent_symbol());
            pb->increment_moves();

            // Recursive call
            int eval = minimax(true, depth + 1, alpha, beta);

            // Undo move
            pb->set_cell_value(r, c, oldVal);
            pb->decrement_moves();

            minEval = min(minEval, eval);
            beta = min(beta, eval);

            // Alpha-beta pruning
            if(beta <= alpha) break;
        }
        return minEval;
    }
}

// Get the best move for the AI player after evaluating all possible moves
Move<char>* PyramidAIPlayer::get_best_move()
{
    PyramidBoard* pb = dynamic_cast<PyramidBoard*>(this->boardPtr);
    if(!pb) return nullptr;

    int bestScore = numeric_limits<int>::min();
    pair<int, int> bestMove = {-1, -1};

    vector<pair<int, int>> moves = pb->get_available_moves();

    cout << this->name << " is thinking..." << endl;

    // Try each possible move
    for(auto& move : moves) {
        int r = move.first;
        int c = move.second;

        // Save old value and make move
        char oldVal = pb->get_cell_value(r, c);
        pb->set_cell_value(r, c, this->symbol);
        pb->increment_moves();

        // Evaluate this move using minimax
        int score = minimax(false, 0, numeric_limits<int>::min(), 
                                      numeric_limits<int>::max());

        // Undo move
        pb->set_cell_value(r, c, oldVal);
        pb->decrement_moves();

        // Update best move
        if(score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    if(bestMove.first == -1) return nullptr;

    cout << this->name << " chose position (" 
         << bestMove.first << ", " << bestMove.second << ")" << endl;

    return new Move<char>(bestMove.first, bestMove.second, this->symbol);
}