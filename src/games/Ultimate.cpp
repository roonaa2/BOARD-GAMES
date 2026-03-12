
#include <iostream>
#include <iomanip>
#include <cctype>  // for toupper()
#include "Ultimate.h"

using namespace std;

//made the board 9x9 with an 2d array to track each sub board
//modified is_win to check each sub-board and then the overall board
//modified get_move to warn if sub-board is already won
//overrided the display ui function

char subboard[3][3]; // to track the status of each 3x3 sub-board 

Ultimate_Board::Ultimate_Board() : Board(9, 9) {

    for (int i = 0; i < 3; ++i)    
        for (int j = 0; j < 3; ++j)      
             subboard[i][j] = blank_symbol; // for the final win check
 
       for (auto& row : board){
            for (auto& cell : row)
               cell = blank_symbol;  //whole board
       }
       
}

bool Ultimate_Board::update_board(Move<char>* move) {
    int x = move->get_x(); //user enters 0..8
    int y = move->get_y();
    char mark = move->get_symbol();


    // Validate move and apply if valid
    if (!(x < 0 || x >= 9 || y < 0 || y >= 9) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) { // Undo move
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {         // Apply move
            n_moves++;
           board[x][y]=toupper(mark);
        }
        return true;
    }
    return false;
}

 

bool Ultimate_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();
  
    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
    };

    // Lambda to check if a 3x3 board is won by symbol s to avoid repeating the code

    auto has_won = [&](char b[3][3], const char& s) {
        // Rows & columns
        for (int i = 0; i < 3; ++i) {
            if (all_equal(b[i][0], b[i][1], b[i][2]) && b[i][0] == s) return true;
            if (all_equal(b[0][i], b[1][i], b[2][i]) && b[0][i] == s) return true;
        }
        //check Diagonals
        if (all_equal(b[0][0], b[1][1], b[2][2]) && b[0][0] == s) return true;
        if (all_equal(b[0][2], b[1][1], b[2][0]) && b[0][2] == s) return true;
        return false;
    };

    
    // Check each sub-board and if it is won, mark it in subboard
   
    for (int sr = 0; sr < 3; ++sr) {
        for (int sc = 0; sc < 3; ++sc) {

            //if  Already won - skip it
            if (subboard[sr][sc] != blank_symbol)
                continue;

            // Extract the 3×3 sub-board
            char small[3][3];
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    small[i][j] = board[sr * 3 + i][sc* 3 + j];
                }
            }

            // If player wins this sub-board → record winner
            if (has_won(small, sym)) {
                subboard[sr][sc] = sym;
            }
        }
    }

  
    // Check the 3×3 subboard for the final winner
   
    return has_won(subboard, sym);
}


bool Ultimate_Board::is_draw(Player<char>* player) {
    return (n_moves == 81 && !is_win(player));
}

bool Ultimate_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}



Ultimate_UI::Ultimate_UI() : UI<char>("Weclome to Ultimate Tic Tac Toe", 3) {
    cout << "- Game Rules:\n"
        << "- Players take turns placing their symbol (X or O) on a 9x9 grid,\n"
       << "- 3x3 grid where each cell contains a smaller, standard 3x3 Tic-Tac-Toe board.\n"
      << "- to win you have to score three sub-boards in a row (horizontally, vertically, or diagonally)\n"
      << "- you have to choose your move from 0-8 to determine which cell in which subboard to play in.\n"
      << "- Good luck and have fun!\n";
}


// override to show sub-board separations

void Ultimate_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    if (matrix.empty() || matrix[0].empty()) return;

    int rows = matrix.size();
    int cols = matrix[0].size();
    const int sub_size = 3; // each sub-board is 3x3

    cout << "\n";

    for (int i = 0; i < rows; ++i) {
        if (i % sub_size == 0) {
            for (int k = 0; k < cols + sub_size - 1; ++k) cout << "--";
            cout << "-\n";
        }

        int sub_row = i / sub_size;

        for (int j = 0; j < cols; ++j) {
            if (j % sub_size == 0) cout << "| ";

            int sub_col = j / sub_size;

            char winner = subboard[sub_row][sub_col]; 
            if (winner != '.') {
                cout << winner << ' ';
            } else {
                cout << matrix[i][j] << ' ';
            }

            if (j == cols - 1) cout << "|";
        }
        cout << "\n";
    }

    for (int k = 0; k < cols + sub_size - 1; ++k) cout << "--";
    cout << "-\n";
}


Player<char>* Ultimate_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* Ultimate_UI::get_move(Player<char>* player) {
    int x, y;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 8): ";
        while (true)
        {
         cin >> x >> y;
        int sr=x/3;
        int sc=y/3;
        if(subboard[sr][sc] != '.'){
            cout << "The sub-board is already won. You can choose any cell.\n";
        }
        else{
            break; //valid move 
        }
    }
}
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}
