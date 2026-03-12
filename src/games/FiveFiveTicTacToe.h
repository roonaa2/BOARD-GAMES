#ifndef FIVEFIVE_TIC_TAC_TOE_H
#define FIVEFIVE_TIC_TAC_TOE_H

#include <BoardGame_Classes.h>
#include <vector>
#include <utility>

using namespace std;

class FiveFiveTicTacToe : public Board<char> {
private:
    char blank_symbol = '.';
    int evaluate_position(); 
    int minimax(int depth, bool isMaximizing, int alpha, int beta);
    int count_three_in_rows(char player_symbol);  
    
public:
    FiveFiveTicTacToe();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; };
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
    void display_board();
  
    pair<int, int> get_best_move(char ai_symbol, char human_symbol);
};

class FiveFiveTicTacToe_UI : public UI<char> {
public:
    FiveFiveTicTacToe_UI();
    ~FiveFiveTicTacToe_UI() {};

    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};

#endif