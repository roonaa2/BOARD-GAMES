#include <iostream>
#include <iomanip>
#include <cctype>   
#include "Misere.h"

using namespace std;

 /**
 I added a helper function to check if a symbol has 3 in a row
 and used it in both is_win and is_lose instead of writing the same code twice
 also chnaged the logic of is_win and is_lose ; u lose if u have 3 in a row (normaly win)
 and win if the other player loses.
 we cant do it directly without the helper function because we only have 
 a ptr to the current player not the other player
 */


MisereBoard::MisereBoard() : Board(3, 3) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool MisereBoard:: has_three(char symbol) {
        auto all_equal = [&](char a, char b, char c) {
            return a == b && b == c && a != blank_symbol;
        };

        // Check rows and columns
        for (int i = 0; i < rows; ++i) {
            if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == symbol) ||
                (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == symbol))
                return true;
        }

        // Check diagonals
        if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == symbol) ||
            (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == symbol))
            return true;

        return false;
    }


bool MisereBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Validate move and apply if valid
  if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol)) {
            n_moves++;
            board[x][y] = toupper(mark);
            return true;
        }
        
    return false;
}

bool MisereBoard::is_lose(Player<char>* player) {
    const char sym = player->get_symbol();
    return (has_three(sym));
  
}

bool MisereBoard::is_win(Player<char>* player) {
    char opponent_symbol = (player->get_symbol() == 'X') ? 'O' : 'X';
    return has_three(opponent_symbol);
}


bool MisereBoard::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_lose(player) &&!is_win(player));
}

bool MisereBoard::game_is_over(Player<char>* player) {
    return is_lose(player) || is_draw(player) || is_win(player);
}

 

MisereUI::MisereUI() : UI<char>("Welcome to Misere Tic Tac Toe", 3) {
    cout << "\nGame Rules:\n";
    cout << "- Players take turns placing their marks (X or O) on the 3x3 grid.\n";
    cout << "- The objective is to avoid creating a line of three of your own marks.\n";
    cout << "- A line can be horizontal, vertical, or diagonal.\n";
    cout << "- If you create a line of three of your marks, you lose the game.\n";
    cout << "- If the board is filled without any player creating such a line, the game ends in a draw.\n\n";   
}

Player<char>* MisereUI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* MisereUI::get_move(Player<char>* player) {
    int x, y;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
           if (x < 0 || x >= 3 || y < 0 || y >= 3) {
            cout << "Move out of bounds. Try again.\n";
            return get_move(player);
    }
    if (player->get_board_ptr()->get_board_matrix()[x][y] != '.') {
            cout << "Cell already occupied. Try again.\n";
            return get_move(player);
    } }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}
