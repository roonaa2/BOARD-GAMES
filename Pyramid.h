#ifndef PYRAMID_H
#define PYRAMID_H
#include <iostream>
#include "BoardGame_Classes.h"
#include <vector>
using namespace std;

class PyramidBoard : public Board<char> {
    private:
        char blank_symbol = '.';
    public:
        PyramidBoard();
        bool update_board(Move<char>* move) override;
        bool is_win(Player<char>* player) override;
        bool is_lose(Player<char>* player) override { return false; }
        bool is_draw(Player<char>* player) override;
        bool game_is_over(Player<char>* player) override;
};

class PyramidUI : public UI<char> {
    public:
        PyramidUI();
        ~PyramidUI() override = default;
        
        Move<char>* get_move(Player<char>* player) override;
        Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
};

#endif
