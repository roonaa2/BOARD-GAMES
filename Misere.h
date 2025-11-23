#ifndef MISERE_H
#define MISERE_H
#include "BoardGame_Classes.h"
using namespace std;
 
class MisereBoard : public Board<char> {
private:
    char blank_symbol = '.';  
    bool has_three(char symbol);
 
public:
     
    MisereBoard();

    bool update_board(Move<char>* move);

    bool is_win(Player<char>* player); 

    bool is_lose(Player<char>*) ;
 
    bool is_draw(Player<char>* player);
 
    bool game_is_over(Player<char>* player);

    
};

 
class MisereUI : public UI<char> {
public:
    
    MisereUI();
 
    ~MisereUI() {};

    Player<char>* create_player(string& name, char symbol, PlayerType type);

    Move<char>* get_move(Player<char>* player);
};

#endif 
