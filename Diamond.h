#ifndef DIAMOND_H
#define DIAMOND_H

#include "BoardGame_Classes.h"
#include <queue>
#include <utility>

using namespace std;

 extern bool valid_cells[7][7];


class DiamondBoard : public Board<char> {
private:
    char blank_symbol = '.';
public:

    bool valid_cells[7][7];
    DiamondBoard();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
};

class Diamond_UI : public UI<char> {
public:
    Diamond_UI();
    ~Diamond_UI() override = default;
    
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
};

#endif 