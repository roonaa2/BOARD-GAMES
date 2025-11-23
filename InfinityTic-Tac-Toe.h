#ifndef INFINITYTIC_TAC_TOE_H
#define INFINITYTIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include <queue>
#include <utility>

using namespace std;

class InfinityBoard : public Board<char> {
private:
    char blank_symbol = '.';
    queue<pair<int, int>> moves_history; //to track the order of moves

public:
    InfinityBoard();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    void remove_oldest_move();  
};

class InfinityTicTacToe_UI : public UI<char> {
public:
    InfinityTicTacToe_UI();
    ~InfinityTicTacToe_UI() override = default;
    
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
};

#endif