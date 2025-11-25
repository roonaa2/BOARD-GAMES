/**
 * @file SUS.h
 * @brief Defines the SUS Tic-Tac-Toe game classes.
 *
 * This file provides:
 * - `SUSBoard`: A specialized board class for the SUS game.
 * - `SUS_UI`: A user interface class tailored to SUS game setup and player interaction.
 * 
 * Game Rules:
 * - Players take turns placing either "S" or "U" in empty squares
 * - A player must use the same letter each turn
 * - A player scores a point for each "S-U-S" sequence they create (horizontally, vertically, or diagonally)
 * - The game ends when the board is full
 * - The player with the most "S-U-S" sequences wins
 */
#ifndef SUS_H
#define SUS_H

#include "BoardGame_Classes.h"
#include <vector>
#include <iostream>

using namespace std;

/**
 * @class SUSBoard
 * @brief Represents the SUS Tic-Tac-Toe game board.
 *
 * This class inherits from the generic `Board<char>` class and implements
 * the specific logic required for the SUS game where players form "S-U-S" sequences.
 *
 * @see Board
 */
class SUSBoard : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell.
    int player1_score = 0;   ///< Score for Player 1 (X)
    int player2_score = 0;   ///< Score for Player 2 (O)

    /**
     * @brief Counts new "S-U-S" sequences formed by the last move.
     * @param x Row of the last move.
     * @param y Column of the last move.
     * @return Number of new "S-U-S" sequences formed.
     */
    int count_new_sus_sequences(int x, int y);

public:
    /**
     * @brief Default constructor that initializes a 3x3 SUS board.
     */
    SUSBoard();

    /**
     * @brief Updates the board with a player's move.
     * @param move Pointer to a Move<char> object containing move coordinates and symbol (S or U).
     * @return true if the move is valid and successfully applied, false otherwise.
     */
    bool update_board(Move<char>* move);

    /**
     * @brief Checks if the given player has won the game.
     * @param player Pointer to the player being checked.
     * @return true if the player has more "S-U-S" sequences than opponent, false otherwise.
     */
    bool is_win(Player<char>* player);

    /**
     * @brief Checks if the given player has lost the game.
     * @param player Pointer to the player being checked.
     * @return false (not used in SUS logic).
     */
    bool is_lose(Player<char>*) { return false; }

    /**
     * @brief Checks if the game has ended in a draw.
     * @param player Pointer to the player being checked.
     * @return true if board is full and scores are equal, false otherwise.
     */
    bool is_draw(Player<char>* player);

    /**
     * @brief Determines if the game is over.
     * @param player Pointer to the player to evaluate.
     * @return true if the board is full, false otherwise.
     */
    bool game_is_over(Player<char>* player);

    /**
     * @brief Gets the character at a specific board position.
     * @param x Row index.
     * @param y Column index.
     * @return Character at the position.
     */
    char get_cell(int x, int y) const;

    /**
     * @brief Gets the score for a specific player.
     * @param player Pointer to the player.
     * @return Player's score.
     */
    int get_score(Player<char>* player) const;
};

/**
 * @class SUS_UI
 * @brief User Interface class for the SUS Tic-Tac-Toe game.
 *
 * Inherits from the generic `UI<char>` base class and provides
 * SUS-specific functionality for player setup and move input.
 *
 * @see UI
 */
class SUS_UI : public UI<char> {
public:
    /**
     * @brief Constructs a SUS_UI object.
     *
     * Initializes the base `UI<char>` class with the welcome message.
     */
    SUS_UI();

    /**
     * @brief Destructor for SUS_UI.
     */
    ~SUS_UI() {}

    /**
     * @brief Creates a player of the specified type.
     * @param name Name of the player.
     * @param symbol Character symbol ('X' or 'O').
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
};

#endif // SUS_H