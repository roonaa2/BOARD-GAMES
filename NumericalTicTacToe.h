/**
 * @file NumericalTicTacToe.h
 * @brief Defines the Numerical Tic-Tac-Toe game classes.
 *
 * This file provides:
 * - `NumericalBoard`: A specialized board class for the Numerical Tic-Tac-Toe game.
 * - `NumericalUI`: A user interface class tailored to Numerical game setup and player interaction.
 */
#ifndef NUMERICAL_TIC_TAC_TOE_H
#define NUMERICAL_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include <vector>
#include <iostream>
#include <cctype>

using namespace std;

/**
 * @class NumericalBoard
 * @brief Represents the Numerical Tic-Tac-Toe game board.
 *
 * This class inherits from the generic `Board<char>` class and implements
 * the specific logic required for the Numerical Tic-Tac-Toe game.
 * Player 1 uses odd numbers (1,3,5,7,9), Player 2 uses even numbers (2,4,6,8).
 * The goal is to make a row, column, or diagonal sum to exactly 15.
 *
 * @see Board
 */
class NumericalBoard : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell.
    vector<int> used_numbers; ///< Track which numbers have been used.

public:
    /**
     * @brief Default constructor that initializes a 3x3 Numerical board.
     */
    NumericalBoard();

    /**
     * @brief Updates the board with a player's move.
     * @param move Pointer to a Move<char> object containing move coordinates and number symbol.
     * @return true if the move is valid and successfully applied, false otherwise.
     */
    bool update_board(Move<char>* move);

    /**
     * @brief Checks if the given player has won the game (made a line sum to 15).
     * @param player Pointer to the player being checked.
     * @return true if the player has a winning line, false otherwise.
     */
    bool is_win(Player<char>* player);

    /**
     * @brief Checks if the given player has lost the game.
     * @param player Pointer to the player being checked.
     * @return Always returns false (not used in Numerical logic).
     */
    bool is_lose(Player<char>*) { return false; }

    /**
     * @brief Checks if the game has ended in a draw.
     * @param player Pointer to the player being checked.
     * @return true if all cells are filled and no player has won, false otherwise.
     */
    bool is_draw(Player<char>* player);

    /**
     * @brief Determines if the game is over (win or draw).
     * @param player Pointer to the player to evaluate.
     * @return true if the game has ended, false otherwise.
     */
    bool game_is_over(Player<char>* player);

    /**
     * @brief Checks if a number has already been used.
     * @param number The number to check.
     * @return true if the number has been used, false otherwise.
     */
    bool is_number_used(int number);
    
    /**
     * @brief Gets the character at a specific board position.
     * @param x Row index.
     * @param y Column index.
     * @return Character at the position.
     */
    char get_cell(int x, int y) const;
};

/**
 * @class NumericalUI
 * @brief User Interface class for the Numerical Tic-Tac-Toe game.
 *
 * Inherits from the generic `UI<char>` base class and provides
 * Numerical-specific functionality for player setup and move input.
 *
 * @see UI
 */
class NumericalUI : public UI<char> {
public:
    /**
     * @brief Constructs a NumericalUI object.
     *
     * Initializes the base `UI<char>` class with the welcome message.
     */
    NumericalUI();

    /**
     * @brief Destructor for NumericalUI.
     */
    ~NumericalUI() {}

    /**
     * @brief Creates a player of the specified type.
     * @param name Name of the player.
     * @param symbol Character symbol ('X' for odd numbers, 'O' for even numbers).
     * @param type The type of the player (Human or Computer).
     * @return Pointer to the newly created Player<char> instance.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type);

    /**
     * @brief Retrieves the next move from a player.
     * @param player Pointer to the player whose move is being requested.
     * @return A pointer to a new `Move<char>` object representing the player's action.
     */
    Move<char>* get_move(Player<char>* player);

private:
    /**
     * @brief Validates if a number choice is valid for the given player.
     * @param number The number chosen.
     * @param player The player making the move.
     * @param board The game board.
     * @return true if valid, false otherwise.
     */
    bool is_valid_number(int number, Player<char>* player, NumericalBoard* board);
};

#endif // NUMERICAL_TIC_TAC_TOE_H