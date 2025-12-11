/**
 * @file ObstaclesTicTacToe.h
 * @brief Defines the Obstacles Tic-Tac-Toe game classes.
 *
 * This file provides:
 * - `ObstaclesBoard`: A specialized board class for the Obstacles game.
 * - `ObstaclesUI`: A user interface class tailored to Obstacles game setup and player interaction.
 * 
 * Game Rules:
 * - A 6x6 grid
 * - After every round (one turn for each player), two new obstacle cells are randomly added
 * - Obstacles (#) cannot be used by either player
 * - First player to align four marks in a row (horizontally, vertically, or diagonally) wins
 * - The game is a draw if the board fills without a winner
 */
#ifndef OBSTACLES_TIC_TAC_TOE_H
#define OBSTACLES_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include <vector>
#include <iostream>
#include <utility>

using namespace std;

/**
 * @class ObstaclesBoard
 * @brief Represents the Obstacles Tic-Tac-Toe game board.
 *
 * This class inherits from the generic `Board<char>` class and implements
 * the specific logic required for the Obstacles game with dynamic obstacle placement.
 *
 * @see Board
 */
class ObstaclesBoard : public Board<char> {
private:
    char blank_symbol = '.';      ///< Character used to represent an empty cell.
    char obstacle_symbol = '#';   ///< Character used to represent an obstacle.
    int round_counter = 0;        ///< Tracks complete rounds (both players moved)
    vector<pair<int, int>> empty_cells; ///< Track empty cells for obstacle placement

    /**
     * @brief Adds two random obstacles to the board.
     */
    void add_random_obstacles();

    /**
     * @brief Updates the list of empty cells.
     */
    void update_empty_cells();

public:
    /**
     * @brief Default constructor that initializes a 6x6 Obstacles board.
     */
    ObstaclesBoard();

    /**
     * @brief Updates the board with a player's move.
     * @param move Pointer to a Move<char> object containing move coordinates and symbol.
     * @return true if the move is valid and successfully applied, false otherwise.
     */
    bool update_board(Move<char>* move);

    /**
     * @brief Checks if there are four in a row for a specific symbol.
     * @param symbol The symbol to check for.
     * @return true if four in a row found, false otherwise.
     */
    bool check_four_in_row(char symbol);

    /**
     * @brief Checks if the given player has won the game (four in a row).
     * @param player Pointer to the player being checked.
     * @return true if the player has four in a row, false otherwise.
     */
    bool is_win(Player<char>* player);

    /**
     * @brief Checks if the given player has lost the game.
     * @param player Pointer to the player being checked.
     * @return false (not used in Obstacles logic).
     */
    bool is_lose(Player<char>*) { return false; }

    /**
     * @brief Checks if the game has ended in a draw.
     * @param player Pointer to the player being checked.
     * @return true if board is full and no player has won, false otherwise.
     */
    bool is_draw(Player<char>* player);

    /**
     * @brief Determines if the game is over.
     * @param player Pointer to the player to evaluate.
     * @return true if the game has ended, false otherwise.
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
     * @brief Gets the number of empty cells (for checking if obstacles can be added).
     * @return Number of empty cells.
     */
    int get_empty_count() const;
    
    /**
     * @brief Sets a cell temporarily (for AI simulation).
     * @param x Row index.
     * @param y Column index.
     * @param value Character to set.
     */
    void set_cell_temp(int x, int y, char value);
};

/**
 * @class ObstaclesUI
 * @brief User Interface class for the Obstacles Tic-Tac-Toe game.
 *
 * Inherits from the generic `UI<char>` base class and provides
 * Obstacles-specific functionality for player setup and move input.
 *
 * @see UI
 */
class ObstaclesUI : public UI<char> {
public:
    /**
     * @brief Constructs an ObstaclesUI object.
     *
     * Initializes the base `UI<char>` class with the welcome message.
     */
    ObstaclesUI();

    /**
     * @brief Destructor for ObstaclesUI.
     */
    ~ObstaclesUI() {}

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

#endif // OBSTACLES_TIC_TAC_TOE_H