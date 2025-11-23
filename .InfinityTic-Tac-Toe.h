#ifndef INFINITYTIC_TAC_TOE_H
#define INFINITYTIC_TAC_TOE_H

#include "BoardGame_Classes.h"
using namespace std;

class InfinityTicTacToe : public Board<char> {

    private:
    char blank_symbol = '.';

    public:
    InfinityTicTacToe();

    bool update_board(Move<char>* move);

    bool is_win(Player<char>* player);

    bool is_lose(Player<char>*) { return false;};

    bool is_draw(Player<char>* player);

    bool game_is_over(Player<char>* player);
};


class infintyTicTacToePlayer_UI : public UI<char>{
    infintyTicTacToePlayer_UI();

    ~infintyTicTacToePlayer_UI() {};

    Player<char>* create_player(string& name, char symbol, PlayerType type);
    Move<char>* get_move(Player<char>* player);


} 
#endif