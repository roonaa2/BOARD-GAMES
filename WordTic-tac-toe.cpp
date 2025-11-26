#include <iostream>
#include <iomanip>
#include <cctype>  
#include "WordTic-tac-toe.h"
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

WORDTIC_TAC_TOE::WORDTIC_TAC_TOE() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
    
    ifstream file("dic.txt");
    string word;
    while (getline(file, word)) {
        word.erase(remove(word.begin(), word.end(), '\r'), word.end());
        word.erase(remove(word.begin(), word.end(), '\n'), word.end());
        if (!word.empty()) {
            words.push_back(word);
        }
    }
    file.close();
}

bool WORDTIC_TAC_TOE::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();
    
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol)) {

        n_moves++;
        board[x][y] = toupper(mark);
        
        // Display the board
        display_board();
        return true;
    }
    return false;
}

void WORDTIC_TAC_TOE::display_board() {
    cout << "\nCurrent Board:\n";
    cout << "  0 1 2\n";
    for (int i = 0; i < 3; i++) {
        cout << i << " ";
        for (int j = 0; j < 3; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool WORDTIC_TAC_TOE::is_valid_word(const string& word) {
    for (const string& dict_word : words) {
        if (dict_word == word) {
            return true;
        }
    }
    return false;
}

vector<string> WORDTIC_TAC_TOE::check_board_words() {
    vector<string> found_words;
   
    for (int i = 0; i < 3; i++) {
        string row_word;
        for (int j = 0; j < 3; j++) {
            row_word += board[i][j];
        }
        if (row_word.find('.') == string::npos && is_valid_word(row_word)) {
            found_words.push_back(row_word);
        }
    }
    
    for (int j = 0; j < 3; j++) {
        string col_word;
        for (int i = 0; i < 3; i++) {
            col_word += board[i][j];
        }
        if (col_word.find('.') == string::npos && is_valid_word(col_word)) {
            found_words.push_back(col_word);
        }
    }

    string diag1, diag2;
    for (int i = 0; i < 3; i++) {
        diag1 += board[i][i];
        diag2 += board[i][2-i];
    }
    
    if (diag1.find('.') == string::npos && is_valid_word(diag1)) {
        found_words.push_back(diag1);
    }
    if (diag2.find('.') == string::npos && is_valid_word(diag2)) {
        found_words.push_back(diag2);
    }
    
    return found_words;
}

bool WORDTIC_TAC_TOE::is_win(Player<char>* player) {
    vector<string> found_words = check_board_words();
    
    if (!found_words.empty()) {
        cout << "\n*** WINNER! Player " << player->get_name() << " formed: ";
        for (const string& word : found_words) {
            cout << word << " ";
        }
        cout << "***\n";
        return true;
    }
    return false;
}

bool WORDTIC_TAC_TOE::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool WORDTIC_TAC_TOE::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

WORDTIC_TAC_TOE_UI::WORDTIC_TAC_TOE_UI() : UI<char>("Welcome to Word Tic-tac-toe", 3) {}

Player<char>* WORDTIC_TAC_TOE_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* WORDTIC_TAC_TOE_UI::get_move(Player<char>* player) {
    int x, y;
    char letter;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlayer " << player->get_name() << " (" << player->get_symbol() << ")";
        cout << "\nPlease enter your move (x y letter): ";
        cin >> x >> y >> letter;
        
        if (!isalpha(letter)) {
            cout << "Wrong letter! Please enter a letter A-Z\n";
            return get_move(player);
        }
    }
    else {
        x = rand() % 3;
        y = rand() % 3;
        letter = 'A' + (rand() % 26);
        cout << "\nComputer chooses position (" << x << ", " << y << ") with letter " << letter << endl;
    }
    
    return new Move<char>(x, y, toupper(letter));
}