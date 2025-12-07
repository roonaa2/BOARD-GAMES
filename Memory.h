#ifndef MEMORY_H
#define MEMORY_H

#include "BoardGame_Classes.h"
#include <queue>
#include <utility>
#include <string>

extern char hiddenBoard[3][3];

using namespace std;

class MemoryBoard : public Board<char> {
private:
    char blank_symbol = '.',hide_symbol = '?';
   
   
public:
    MemoryBoard();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
};

class Memory_UI : public UI<char> {
public:
    Memory_UI();
    ~Memory_UI() override = default;
    
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
};

#endif 