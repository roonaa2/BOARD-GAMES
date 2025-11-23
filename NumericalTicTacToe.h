
/**  
#ifndef _NUMERICAL_TIC_TAC_TOE_H
#define _NUMERICAL_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>

using namespace std;

class NumericalTicTacToe : public Board<int> {
private:
    static const int GRID_SIZE = 3;

public:
    NumericalTicTacToe() {
        rows = GRID_SIZE;
        columns = GRID_SIZE;
        board = new int*[GRID_SIZE];
        for (int i = 0; i < GRID_SIZE; i++) {
            board[i] = new int[GRID_SIZE]{0};
        }
        n_moves = 0;
    }

    ~NumericalTicTacToe() {
        for (int i = 0; i < GRID_SIZE; i++) {
            delete[] board[i];
        }
        delete[] board;
    }

    bool update_board(int row, int col, int value) override {
        if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE || board[row][col] != 0) {
            return false;
        }
        board[row][col] = value;
        n_moves++;
        return true;
    }

    void display_board() override {
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    bool is_win() override {
        for (int i = 0; i < GRID_SIZE; i++) {
            int rowSum = 0, colSum = 0;
            for (int j = 0; j < GRID_SIZE; j++) {
                rowSum += board[i][j];
                colSum += board[j][i];
            }
            if (rowSum == 15 || colSum == 15) {
                return true;
            }
        }

        int mainDiagonalSum = 0, antiDiagonalSum = 0;
        for (int i = 0; i < GRID_SIZE; i++) {
            mainDiagonalSum += board[i][i];
            antiDiagonalSum += board[i][GRID_SIZE - i - 1];
        }

        return mainDiagonalSum == 15 || antiDiagonalSum == 15;
    }

    bool is_draw() override {
        return n_moves == GRID_SIZE * GRID_SIZE && !is_win();
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }
};

class NumericalTicTacToePlayer : public Player<int> {
public:
    NumericalTicTacToePlayer(string playerName, int playerSymbol) : Player<int>(playerName, playerSymbol) {}

    void getmove(int& row, int& col) override {
        int number;
        cout << getname() << ": Enter your move (row column number): ";
        cin >> row >> col >> number;

        while (!is_valid_move(number) || !boardPtr->update_board(row, col, number)) {
            cout << "Invalid move. Try again: ";
            cin >> row >> col >> number;
        }
    }

private:
    bool is_valid_move(int number) {
        if (symbol % 2 == 1 && number % 2 == 0) {
            cout << "You must enter an odd number!\n";
            return false;
        }
        if (symbol % 2 == 0 && number % 2 != 0) {
            cout << "You must enter an even number!\n";
            return false;
        }
        return true;
    }
};

class NumericalTicTacToeManager {
private:
    NumericalTicTacToe* gameBoardPtr;
    NumericalTicTacToePlayer* players[2];

public:
    NumericalTicTacToeManager(NumericalTicTacToe* board, NumericalTicTacToePlayer* player1, NumericalTicTacToePlayer* player2) {
        gameBoardPtr = board;
        players[0] = player1;
        players[1] = player2;
        players[0]->setBoard(board);
        players[1]->setBoard(board);
    }

    void run() {
        int row, col;
        gameBoardPtr->display_board();

        while (!gameBoardPtr->game_is_over()) {
            for (int i = 0; i < 2; i++) {
                players[i]->getmove(row, col);
                gameBoardPtr->display_board();
                if (gameBoardPtr->is_win()) {
                    cout << players[i]->getname() << " wins!\n";
                    return;
                }
                if (gameBoardPtr->is_draw()) {
                    cout << "It's a draw!\n";
                    return;
                }
            }
        }
    }
};

#endif // _NUMERICAL_TIC_TAC_TOE_H
*/
