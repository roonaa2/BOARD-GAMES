#ifndef INFINITYTIC_TAC_TOE_H
#define INFINITYTIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include <queue>
#include <utility>

using namespace std;

class InfinityBoard : public Board<char> {
private:
    char blank_symbol = '.';
    queue<pair<int, int>> moves_history; 
    
public:
    InfinityBoard();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    void remove_oldest_move();
    
    // Only expose blank_symbol since parent Board already has get_cell()
    char get_blank_symbol() const { return blank_symbol; }
};

class InfinityTicTacToe_AI : public Player<char> {
public:
    InfinityTicTacToe_AI(string name, char symbol);
    
    // Override get_move to provide AI logic for move selection
    Move<char>* get_move();
    
private:
    // AI strategy methods for move selection
    Move<char>* find_winning_move();
    Move<char>* find_blocking_move();
    Move<char>* find_strategic_move();
    Move<char>* find_center_or_corner();
    
    int evaluate_position(int x, int y);
    bool can_win_in_line(int x1, int y1, int x2, int y2, int x3, int y3, char symbol);
    bool is_valid_move(int x, int y);
    int count_in_line(int x1, int y1, int x2, int y2, int x3, int y3, char symbol);
};

class InfinityTicTacToe_UI : public UI<char> {
public:
    InfinityTicTacToe_UI();
    ~InfinityTicTacToe_UI() override = default;
    
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
};

#endif