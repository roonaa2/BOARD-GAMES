

#ifndef ULTIMATE_CLASSES_H
#define ULTIMATE_H

#include <BoardGame_Classes.h>
using namespace std;

class  Ultimate_Board : public Board<char> {
private:
    char blank_symbol = '.';  

public:
    
     Ultimate_Board();
 
    bool update_board(Move<char>* move);

  
    bool is_win(Player<char>* player);

   
    bool is_lose(Player<char>*) { return false; };

    bool is_draw(Player<char>* player);

    bool game_is_over(Player<char>* player);
};


 
class  Ultimate_UI : public UI<char> {
public:
     
     Ultimate_UI();

 
    ~Ultimate_UI() {};

  
    Player<char>* create_player(string& name, char symbol, PlayerType type);

 
    virtual Move<char>* get_move(Player<char>* player);

    void display_board_matrix(const vector<vector<char>>& matrix) const override ;
};

#endif 
