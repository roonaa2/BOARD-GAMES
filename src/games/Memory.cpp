
#include <iostream>
#include <iomanip>
#include <cctype>  
#include "Memory.h"


using namespace std;

 /** 
   I added a 3x3 hiddenBoard to store the actual symbols
  also added hide symbol and modified update_board accordingly 
  ui will deal with the normal board which has hide symbol and 
  is_win will check the hiddenBoard for matches

 */
  char hiddenBoard[3][3]= {
        {'.', '.', '.'},
        {'.', '.', '.'},
        {'.', '.', '.'}
    };
MemoryBoard::MemoryBoard() : Board(3, 3) {
  
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool MemoryBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

   
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol)) {
            n_moves++;
            board[x][y] = hide_symbol;
            hiddenBoard[x][y] = toupper(mark);
        return true;

    }
    return false;
}
 

bool MemoryBoard::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
        };

    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if ((all_equal(hiddenBoard[i][0],hiddenBoard[i][1],hiddenBoard[i][2]) && hiddenBoard[i][0] == sym) ||
            (all_equal(hiddenBoard[0][i],hiddenBoard[1][i],hiddenBoard[2][i]) && hiddenBoard[0][i] == sym))
            return true;
    }

    // Check diagonals
    if ((all_equal(hiddenBoard[0][0],hiddenBoard[1][1],hiddenBoard[2][2]) &&hiddenBoard[1][1] == sym) ||
        (all_equal(hiddenBoard[0][2],hiddenBoard[1][1],hiddenBoard[2][0]) && hiddenBoard[1][1] == sym))
        return true; 

    return false;
}

bool MemoryBoard::is_draw(Player<char>* player) {
    return  (!is_win(player) && n_moves >= 9);
}

bool MemoryBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Memory_UI::Memory_UI() : UI<char>("Welcome to Infinity TIC TAC TOE", 3) {
    cout << "-Game Rules:\n";
    cout << "-Each player takes turns to place their mark on an empty cell\n";
    cout << "-Once placed, the mark is hidden and represented by '?'\n";
    cout << "-To win, a player must have 3 of their marks in a row and memorize their positions\n";
}

Player<char>* Memory_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* Memory_UI::get_move(Player<char>* player) {
    int x, y;
    
    if (player->get_type() == PlayerType::HUMAN) {
        while(true) {
        cout << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
        if (x < 0 || x >= 3 || y < 0 || y >= 3) {
            cout << "Move out of bounds. Try again.\n";
            continue;
    }
    if (player->get_board_ptr()->get_board_matrix()[x][y] != '.') {
            cout << "Cell already occupied. Try again.\n";
           continue;
    } 
    break;
}}
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}
