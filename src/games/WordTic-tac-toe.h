#ifndef WORDTIC_TAC_TOE_H
#define WORDTIC_TAC_TOE_H
#include <vector>
#include <string>
#include <BoardGame_Classes.h>
#include <queue>
#include <utility>

using namespace std;

class WORDTIC_TAC_TOE: public Board<char> {
private:
    char blank_symbol = '.'; 
    vector<string> words;
    
    // Helper function declarations
    bool is_valid_word(const string& word);
    vector<string> check_board_words();
    
public:
    WORDTIC_TAC_TOE();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; };
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
    void display_board(); 
};

class WORDTIC_TAC_TOE_UI : public UI<char> {
public:
    WORDTIC_TAC_TOE_UI();
    ~WORDTIC_TAC_TOE_UI() {};

    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};

#endif