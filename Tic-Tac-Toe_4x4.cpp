
#include <iostream>
#include <iomanip>
#include <cctype>  
#include "Tic-Tac-Toe_4x4.h"

using namespace std;

X_O_4x4Board::X_O_4x4Board() : Board(4, 4) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;

    board[0][1] = 'X'; //First row in the game
    board[0][3] = 'X';
    board[0][0] = 'O';
    board[0][2] = 'O';

    board[3][0] = 'X'; // Last row in the came
    board[3][2] = 'X';
    board[3][1] = 'O';
    board[3][3] = 'O';
}

bool X_O_4x4Board::update_board(Move<char>* move) {
    int tx = move->get_x();
    int ty = move->get_y();
    char mark = move->get_symbol();

    // Check if move is within bounds
    if (tx < 0 || tx >= rows || ty < 0 || ty >= columns) {
        cout << "Move out of bounds. Try again.\n";
        return false;}

    if (board[tx][ty] != blank_symbol) {
        cout << "Cell already occupied. Try again.\n";
        return false;
    }

    // Search for an adjacent piece belonging to the player to move based on adjacency rule
    int dirs[4][2] = {
        {1,0},  // down
        {-1,0}, // up
        {0,1},  // right
        {0,-1}  // left
    };

    int old_x = -1, old_y = -1;

    // Look for any adjacent piece to determine if the move is legal
    for (auto &d : dirs) {
        int nx = tx + d[0];
        int ny = ty + d[1];

        if (nx >= 0 && nx < rows && ny >= 0 && ny < columns) {
            if (board[nx][ny] == mark) {
                old_x = nx;
                old_y = ny;
                break;
            }
        }
    }

    if (old_x == -1) {
        cout << "You must move next to one of your own pieces.\n";
        return false;
    }

    //Remove the old piece 
    board[old_x][old_y] = blank_symbol;

    //Place the piece in the new location
    board[tx][ty] = mark;

    return true;
}




bool X_O_4x4Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c ) {
        return a == b && b == c && a != blank_symbol;
        };

    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if ((all_equal(board[i][0], board[i][1], board[i][2] ) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i] ) && board[0][i] == sym))
            return true;
    }

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

bool X_O_4x4Board::is_draw(Player<char>* player){
    for (int i= 0 ; i < rows; ++i){
        for (int j=0; j < columns; ++j){
            if (board[i][j] == blank_symbol)
                return false;
        }
    } return true; // this means no cells are blank
}

bool X_O_4x4Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}





X_O_4x4_UI::X_O_4x4_UI() : UI<char>("Weclome to FCAI X-O 4x4 Game", 3) {
    cout << "X-O 4x4 UI initialized.\n";
    cout << "Players will take turns moving their pieces on a 4x4 board.\n";
    cout << "To make a move, enter the coordinates of the cell where you want to place your piece.\n";
    cout << "You can only move to an adjacent cell next to one of your existing pieces.\n";
    cout << "The goal is to align three of your pieces in a row, column, or diagonal.\n";
    cout << "Ready?, Enjoy the game!\n";
}

Player<char>* X_O_4x4_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* X_O_4x4_UI::get_move(Player<char>* player) {
    int x, y;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}

