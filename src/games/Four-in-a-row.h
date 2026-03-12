#ifndef FOUR_IN_A_ROW_H
#define FOUR_IN_A_ROW_H

#include "BoardGame_Classes.h"
#include <vector>

using namespace std;

class FourInARowBoard : public Board<char> {
private:
    char blank_symbol = '.';
    
public:
    FourInARowBoard();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    
    char get_blank_symbol() const { return blank_symbol; }
    int find_lowest_row(int column);
};

class FourInARow_AI : public Player<char> {
public:
    FourInARow_AI(string name, char symbol);
    Move<char>* get_move();
    
private:
    // AI strategy methods
    Move<char>* find_winning_move();
    Move<char>* find_blocking_move();
    Move<char>* find_strategic_move();
    Move<char>* find_center_column();
    
    // Helper methods
    int evaluate_column(int col);
    bool can_win_with_move(int row, int col, char symbol);
    bool is_valid_column(int col);
    int count_direction(int row, int col, int dr, int dc, char symbol);
    int evaluate_position(int row, int col);
    bool check_four_in_line(int row, int col, char symbol);
};

class FourInARow_UI : public UI<char> {
public:
    FourInARow_UI();
    ~FourInARow_UI() override = default;
    
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
};

#endif