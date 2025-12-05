#ifndef TIC_TAC_TOE_4X4_H
#define TIC_TAC_TOE_4X4_H

#include "BoardGame_Classes.h"
#include <vector>
#include <limits>

using namespace std;

// Forward declaration for AI player
class X_O_4x4_AI_Player;

class X_O_4x4Board : public Board<char> {
private:
    char blank_symbol = '.';    
    
public:
    X_O_4x4Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    
    // Helper methods for AI
    vector<Move<char>*> get_valid_moves(char symbol);
    pair<int, int> simulate_move(Move<char>* move);
    void undo_move(Move<char>* move, pair<int, int> old_pos);
    bool check_win_for_symbol(char symbol);
    char get_blank_symbol() const { return blank_symbol; }
};

class X_O_4x4_UI : public UI<char> {
public:
    X_O_4x4_UI();
    ~X_O_4x4_UI() override = default;
    
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
};

/**
 * AI Player class that uses Minimax algorithm with alpha-beta pruning
 * to find optimal moves in 4x4 Tic-Tac-Toe
 */
class X_O_4x4_AI_Player : public Player<char> {
private:
    static const int MAX_DEPTH = 6; // Maximum search depth for minimax
    int minimax(X_O_4x4Board* board, int depth, bool is_maximizing, 
                int alpha, int beta, char maximizer_symbol, char minimizer_symbol);
    int evaluate_board(X_O_4x4Board* board, char maximizer_symbol, char minimizer_symbol);
    
public:
    X_O_4x4_AI_Player(string name, char symbol);
    Move<char>* get_move();
};

#endif