#include <iostream>
#include <iomanip>
#include <cctype>
#include <limits>
#include <algorithm>
#include "FiveFiveTicTacToe.h"
using namespace std;



FiveFiveTicTacToe::FiveFiveTicTacToe() : Board(5, 5) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool FiveFiveTicTacToe::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol)) {

        n_moves++;
        board[x][y] = toupper(mark);
        return true;
    }
    return false;
}

void FiveFiveTicTacToe::display_board() {
    cout << "\nCurrent Board (5x5):\n";
    cout << "  0 1 2 3 4\n";
    for (int i = 0; i < 5; i++) {
        cout << i << " ";
        for (int j = 0; j < 5; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int FiveFiveTicTacToe::count_three_in_rows(char player_symbol) {
    int count = 0;
    player_symbol = toupper(player_symbol);

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) { 
            if (board[i][j] == player_symbol && 
                board[i][j+1] == player_symbol && 
                board[i][j+2] == player_symbol) {
                count++;
            }
        }
    }
    
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 3; i++) {  
            if (board[i][j] == player_symbol && 
                board[i+1][j] == player_symbol && 
                board[i+2][j] == player_symbol) {
                count++;
            }
        }
    }
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == player_symbol && 
                board[i+1][j+1] == player_symbol && 
                board[i+2][j+2] == player_symbol) {
                count++;
            }
        }
    }
    
    for (int i = 0; i < 3; i++) {
        for (int j = 2; j < 5; j++) {
            if (board[i][j] == player_symbol && 
                board[i+1][j-1] == player_symbol && 
                board[i+2][j-2] == player_symbol) {
                count++;
            }
        }
    }
    
    return count;
}

bool FiveFiveTicTacToe::is_win(Player<char>* player) {
    if (n_moves >= 24) {
        char ai_symbol = player->get_symbol();
        char opponent_symbol = (ai_symbol == 'X') ? 'O' : 'X';
        
        int ai_score = count_three_in_rows(ai_symbol);
        int opponent_score = count_three_in_rows(opponent_symbol);
        
        if (ai_score > opponent_score) {
            cout << "\n*** WINNER! Player " << player->get_name() 
                 << " wins with " << ai_score << " three-in-a-rows! ***\n";
            return true;
        }
    }
    return false;
}

bool FiveFiveTicTacToe::is_draw(Player<char>* player) {
    if (n_moves >= 24) {
        char ai_symbol = player->get_symbol();
        char opponent_symbol = (ai_symbol == 'X') ? 'O' : 'X';
        
        int ai_score = count_three_in_rows(ai_symbol);
        int opponent_score = count_three_in_rows(opponent_symbol);
        
        if (ai_score == opponent_score) {
            cout << "\n*** It's a DRAW! Both players have " 
                 << ai_score << " three-in-a-rows. ***\n";
            return true;
        }
    }
    return false;
}

bool FiveFiveTicTacToe::game_is_over(Player<char>* player) {
    return (n_moves >= 24) || is_win(player) || is_draw(player);
}

int FiveFiveTicTacToe::evaluate_position() {
    int score = 0;
    
    if (board[2][2] == 'X') score += 3;
    else if (board[2][2] == 'O') score -= 3;
    
    int x_score = count_three_in_rows('X');
    int o_score = count_three_in_rows('O');
    
    score += (x_score - o_score) * 10;
    
    return score;
}

int FiveFiveTicTacToe::minimax(int depth, bool isMaximizing, int alpha, int beta) {
    if (depth == 0 || n_moves >= 24) {
        return evaluate_position();
    }
    
    if (isMaximizing) {
        int maxEval = -10000;
        
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (board[i][j] == '.') {
                    board[i][j] = 'X';
                    n_moves++;
                    
                    int eval = minimax(depth - 1, false, alpha, beta);
                    
                    board[i][j] = '.';
                    n_moves--;
                    
                    maxEval = max(maxEval, eval);
                    alpha = max(alpha, eval);
                    
                    if (beta <= alpha) break;
                }
            }
        }
        return maxEval;
    } else {
        int minEval = 10000;
        
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (board[i][j] == '.') {
                    board[i][j] = 'O';
                    n_moves++;
                    
                    int eval = minimax(depth - 1, true, alpha, beta);
                    
                    board[i][j] = '.';
                    n_moves--;
                    
                    minEval = min(minEval, eval);
                    beta = min(beta, eval);
                    
                    if (beta <= alpha) break;
                }
            }
        }
        return minEval;
    }
}

pair<int, int> FiveFiveTicTacToe::get_best_move(char ai_symbol, char human_symbol) {
    int bestScore = (ai_symbol == 'X') ? -10000 : 10000;
    pair<int, int> bestMove = make_pair(-1, -1);
    
    int depth = 3;
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] == '.') {
                board[i][j] = ai_symbol;
                n_moves++;
                
                int score = minimax(depth - 1, (human_symbol == 'X'), -10000, 10000);
                
                board[i][j] = '.';
                n_moves--;
                
                if (ai_symbol == 'X') {
                    if (score > bestScore) {
                        bestScore = score;
                        bestMove = make_pair(i, j);
                    }
                } else {
                    if (score < bestScore) {
                        bestScore = score;
                        bestMove = make_pair(i, j);
                    }
                }
            }
        }
    }
    
    if (bestMove.first == -1) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (board[i][j] == '.') {
                    return make_pair(i, j);
                }
            }
        }
    }
    
    return bestMove;
}

FiveFiveTicTacToe_UI::FiveFiveTicTacToe_UI() : UI<char>("Welcome to 5x5 Tic-Tac-Toe", 5) {}

Player<char>* FiveFiveTicTacToe_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* FiveFiveTicTacToe_UI::get_move(Player<char>* player) {
    int x, y;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlayer " << player->get_name() << " (" << player->get_symbol() << ")";
        cout << "\nEnter your move (x y from 0-4): ";
        cin >> x >> y;
        
        while (x < 0 || x > 4 || y < 0 || y > 4) {
            cout << "Invalid position! Enter x y (0-4): ";
            cin >> x >> y;
        }
    }
    else {
        cout << "\nComputer is thinking...\n";
        
        char ai_symbol = player->get_symbol();
        char human_symbol = (ai_symbol == 'X') ? 'O' : 'X';
        
        FiveFiveTicTacToe* game_board = dynamic_cast<FiveFiveTicTacToe*>(player->get_board_ptr());
        
        if (game_board) {
            pair<int, int> bestMove = game_board->get_best_move(ai_symbol, human_symbol);
            x = bestMove.first;
            y = bestMove.second;
        } else {
            x = rand() % 5;
            y = rand() % 5;
        }
        
        cout << "\nComputer chooses position (" << x << ", " << y << ")\n";
    }
    
    return new Move<char>(x, y, player->get_symbol());
}
