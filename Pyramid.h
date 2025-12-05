#ifndef PYRAMID_H
#define PYRAMID_H

#include "BoardGame_Classes.h"
#include <vector>
using namespace std;

class PyramidBoard : public Board<char>
{
private:
    char blank = '.';
    vector<vector<bool>> valid;

public:
    PyramidBoard();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    bool is_valid_position(int r, int c) const;
    bool is_cell_empty(int r, int c) const;
    
    // Public methods for AI to access board state safely
    vector<pair<int, int>> get_available_moves() const;
    char get_cell_value(int r, int c) const;
    void set_cell_value(int r, int c, char value);
    void increment_moves();
    void decrement_moves();
};

class PyramidUI : public UI<char>
{
public:
    PyramidUI();

    Move<char>* get_move(Player<char>* player) override;
    Player<char>** setup_players() override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

class PyramidAIPlayer : public Player<char>
{
private:
    // Minimax algorithm with alpha-beta pruning
    int minimax(bool isMaximizing, int depth, int alpha, int beta);
    
    // Evaluate current board state
    int evaluate();
    
    // Helper to get opponent's symbol
    char get_opponent_symbol() const;

public:
    // Constructor matches base Player class signature
    PyramidAIPlayer(string name, char symbol);
    
    // Virtual destructor
    virtual ~PyramidAIPlayer() {}

    // Get the best move using minimax (NOT an override)
    Move<char>* get_best_move();
};

#endif