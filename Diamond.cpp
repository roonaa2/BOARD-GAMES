#include <iostream>
#include <iomanip>
#include <cctype>  
#include "Diamond.h"
using namespace std;

/**
I made the board 7x7 to make a 5x5 diamond shape by making the invalid cells invisible
also added a boolean array to track valid cells (easier than a vector and a helper function)
 */

  bool valid_cells[7][7] = {
    {false, false, false, true,  false, false, false},
    {false, false, true,  true,  true,  false, false},
    {false, true,  true,  true,  true,  true,  false},
    {true,  true,  true,  true,  true,  true,  true },
    {false, true,  true,  true,  true,  true,  false},
    {false, false, true,  true,  true,  false, false},
    {false, false, false, true,  false, false, false}
};

DiamondBoard::DiamondBoard() : Board(7, 7) {
 
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < columns; y++) {
            if (valid_cells[x][y]) {
                board[x][y] = blank_symbol;
            } else {
                board[x][y] = ' '; // Invalid cells are set to space
            }
        }
    }
}

bool DiamondBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        return false;
    } 
    if (valid_cells[x][y] && (board[x][y] == blank_symbol)) {
        n_moves++;
        board[x][y] = toupper(mark);
        return true;
    }
    return false;
}

// added another lambda to check for lines of 4 also added a struct to store line info 
// and a vector of line info to track found lines of 4 with direction
bool DiamondBoard::is_win(Player<char>* player) {
    char sym = player->get_symbol();
    
    auto all_equal3 = [&](char a, char b, char c) {
        return a == b && b == c && a == sym;
    };
    
    auto all_equal4 = [&](char a, char b, char c, char d) {
        return a == b && b == c && c == d && a == sym;
    };
    
    struct LineInfo {
        vector<pair<int, int>> cells;
        pair<int, int> dir;
    };
    
    vector<LineInfo> found4;
    
    // Directions to check
    vector<pair<int, int>> directions = {{0,1}, {1,0}, {1,1}, {1,-1}};
    
    // Check for lines of 4 first
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < columns; y++) {
             
            if (!valid_cells[x][y] || board[x][y] != sym) continue;
            
            for (const auto& dir : directions) {
                int dr = dir.first, dc = dir.second;
                
                int x1 = x + dr, y1 = y + dc;
                int x2 = x + 2*dr, y2 = y + 2*dc;
                int x3 = x + 3*dr, y3 = y + 3*dc;
                
                // Check bounds for all cells
                if (x1 >= 0 && x1 < rows && y1 >= 0 && y1 < columns &&
                    x2 >= 0 && x2 < rows && y2 >= 0 && y2 < columns &&
                    x3 >= 0 && x3 < rows && y3 >= 0 && y3 < columns) {
                   
                    if (valid_cells[x1][y1] && valid_cells[x2][y2] && valid_cells[x3][y3] &&
                        all_equal4(board[x][y], board[x1][y1], board[x2][y2], board[x3][y3])) {
                        
                        found4.push_back({{{x,y}, {x1,y1}, {x2,y2}, {x3,y3}}, dir});
                    }
                }
            }
        }
    }
    

    if (!found4.empty()) {

    // only check if we have at least 1 line of 4  
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < columns; y++) {
            if (!valid_cells[x][y] || board[x][y] != sym) continue;
            
            for (const auto& dir : directions) {
                int dr = dir.first, dc = dir.second;
                
                int x1 = x + dr, y1 = y + dc;
                int x2 = x + 2*dr, y2 = y + 2*dc;
           
                if (x1 >= 0 && x1 < rows && y1 >= 0 && y1 < columns &&
                    x2 >= 0 && x2 < rows && y2 >= 0 && y2 < columns) {
                  
                    if (valid_cells[x1][y1] && valid_cells[x2][y2] &&
                        all_equal3(board[x][y], board[x1][y1], board[x2][y2])) {
                        
                        vector<pair<int, int>> line3_cells = {{x,y}, {x1,y1}, {x2,y2}};
                        
                        // Check against each found line of 4
                        for (const auto& line4 : found4) {
                           
                            if ((dr == line4.dir.first && dc == line4.dir.second) ||
                                (dr == -line4.dir.first && dc == -line4.dir.second)) {
                                continue;  
                            }
                            
                       
                            int shared = 0;
                            for (const auto& cell3 : line3_cells) {
                                for (const auto& cell4 : line4.cells) {
                                    if (cell3.first == cell4.first && cell3.second == cell4.second) {
                                        shared++;
                                        if (shared > 1) break;
                                    }
                                }
                                if (shared > 1) break;
                            }
                            
                            if (shared <= 1) {
                                return true; 
                            }
                        }
                    }
                }
            }
        }
    }
}
    return false;
}

bool DiamondBoard::is_draw(Player<char>* player) {
    
    return (!is_win(player) && n_moves >= 25);
}

bool DiamondBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Diamond_UI::Diamond_UI() : UI<char>("Welcome to Diamond TIC TAC TOE", 3) {
    cout << "\nGame Rules:\n";
    cout << "- A 7x7 grid arranged in a diamond shape yo make it look like a rotated 5x5 grid\n";
    cout << "- Each turn, you can place your mark in a valid cell only\n";
    cout << "- invalid cells are marked invisible and valid cells are marked with '.'\n";
    cout << "- player can win by completing a line of three marks and a line of four marks\n";
    cout << "- lines must be in different directions but can share 1 common mark.\n";
    cout << "- Game ends when the board is full\n";
    cout << "- U can always undo a move by entering the mark 0\n";
}

Player<char>* Diamond_UI::create_player(string& name, char symbol, PlayerType type) {
   
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";
    
    return new Player<char>(name, symbol, type);
}

 Move<char>* Diamond_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        
/* here I used a simple loop instead of recursion like the other games because with recursion
  it might cause stack overflow after multiple invalid inputs
 */
        while (true) {  
            cout << "\nPlease enter your move x and y (0 to 6): "
                    "(only valid diamond + empty cells marked '.')\n";
            cin >> x >> y;

       
            if (x < 0 || x >= 7 || y < 0 || y >= 7) {
                cout << "Move out of bounds. Try again.\n";
                continue;
            }

            if (!valid_cells[x][y] ||
                player->get_board_ptr()->get_board_matrix()[x][y] != '.') {
                cout << "Cell not valid or already occupied. Try again.\n";
                continue;
            }

            break;
        }
    }



    else if (player->get_type() == PlayerType::COMPUTER) {
        while (true) {
            x = rand() % player->get_board_ptr()->get_rows();
            y = rand() % player->get_board_ptr()->get_columns();

            if (valid_cells[x][y] &&
                player->get_board_ptr()->get_board_matrix()[x][y] == '.') {
                break;
            }
        }
    }

    return new Move<char>(x, y, player->get_symbol());
}
