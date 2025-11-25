#ifndef  TIC_TAC_TOE_4X4_H
#define  TIC_TAC_TOE_4X4_H

#include "BoardGame_Classes.h"
#include <queue>
#include <utility>

using namespace std;

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
    void remove_oldest_move();  
};

class X_O_4x4_UI : public UI<char> {
public:
    X_O_4x4_UI();
    ~X_O_4x4_UI() override = default;
    
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
};

#endif