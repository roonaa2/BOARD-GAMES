/**
 * @file NumericalTicTacToe.h
 * @brief Defines the Numerical Tic-Tac-Toe game classes.
 *
 * This file provides:
 * - `NumericalBoard`: A specialized board class for the Numerical Tic-Tac-Toe game.
 * - `NumericalUI`: A user interface class tailored to Numerical game setup and player interaction.
 * - `NumericalAIPlayer`: An AI player class with different difficulty levels.
 */
#ifndef NUMERICAL_TIC_TAC_TOE_H
#define NUMERICAL_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include <vector>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <limits>

using namespace std;

// AI Difficulty Levels
enum class AIDifficulty {
    EASY,      // Random moves
    MEDIUM,    // Some strategy + random
    HARD,      // Minimax algorithm
    EXPERT     // Minimax with alpha-beta pruning
};

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
     * @brief Copy constructor for simulation
     */
    NumericalBoard(const NumericalBoard& other);

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
    bool is_number_used(int number) const;
    
    /**
     * @brief Gets the character at a specific board position.
     * @param x Row index.
     * @param y Column index.
     * @return Character at the position.
     */
    char get_cell(int x, int y) const;
    
    /**
     * @brief Sets a cell temporarily (for AI simulation).
     * @param x Row index.
     * @param y Column index.
     * @param value Character to set.
     */
    void set_cell_temp(int x, int y, char value);
    
    /**
     * @brief Evaluates how close a line is to summing to 15.
     * @param cells Array of 3 cells to evaluate.
     * @return Score indicating how promising this line is.
     */
    int evaluate_line(char cells[3]) const;
    
    /**
     * @brief Gets available numbers for a player.
     * @param is_odd true for odd numbers, false for even numbers.
     * @return Vector of available numbers.
     */
    vector<int> get_available_numbers(bool is_odd) const;
    
    /**
     * @brief Gets all empty positions on the board.
     * @return Vector of pairs (row, col) for empty cells.
     */
    vector<pair<int, int>> get_empty_positions() const;
    
    /**
     * @brief Evaluates the board state for AI.
     * @param ai_symbol Symbol of the AI player.
     * @param opponent_symbol Symbol of the opponent.
     * @return Evaluation score.
     */
    int evaluate_board(char ai_symbol, char opponent_symbol) const;
    
    /**
     * @brief Checks if a move would create a win.
     * @param x Row position.
     * @param y Column position.
     * @param number Number to place.
     * @return true if the move creates a win.
     */
    bool is_winning_move(int x, int y, char number) const;
    
    /**
     * @brief Checks if a move would block opponent's win.
     * @param x Row position.
     * @param y Column position.
     * @param number Number to place.
     * @param opponent_symbol Opponent's symbol.
     * @return true if the move blocks opponent's win.
     */
    bool is_blocking_move(int x, int y, char number, char opponent_symbol) const;
    
    /**
     * @brief Gets the list of used numbers.
     * @return Vector of used numbers.
     */
    vector<int> get_used_numbers() const { return used_numbers; }
    
    /**
     * @brief Sets the list of used numbers (for simulation).
     * @param numbers Vector of numbers to set.
     */
    void set_used_numbers(const vector<int>& numbers) { used_numbers = numbers; }
};

/**
 * @class NumericalAIPlayer
 * @brief AI player for Numerical Tic-Tac-Toe with different difficulty levels.
 */
class NumericalAIPlayer : public Player<char> {
private:
    AIDifficulty difficulty;
    char opponent_symbol;
    
    /**
     * @brief Gets the best move using Minimax with Alpha-Beta pruning.
     * @param board Current game board.
     * @param depth Current depth in the search tree.
     * @param alpha Alpha value for pruning.
     * @param beta Beta value for pruning.
     * @param is_maximizing Whether this is the maximizing player.
     * @return Pair of (score, move) where move is (row, col, number).
     */
    pair<int, tuple<int, int, char>> minimax(NumericalBoard& board, int depth, int alpha, int beta, bool is_maximizing);
    
    /**
     * @brief Gets the best move for easy difficulty.
     * @param board Current game board.
     * @return Best move (row, col, number).
     */
    tuple<int, int, char> get_easy_move(NumericalBoard& board);
    
    /**
     * @brief Gets the best move for medium difficulty.
     * @param board Current game board.
     * @return Best move (row, col, number).
     */
    tuple<int, int, char> get_medium_move(NumericalBoard& board);
    
    /**
     * @brief Gets the best move for hard difficulty.
     * @param board Current game board.
     * @return Best move (row, col, number).
     */
    tuple<int, int, char> get_hard_move(NumericalBoard& board);
    
public:
    /**
     * @brief Constructor for AI player.
     * @param name Player name.
     * @param symbol Player symbol.
     * @param difficulty AI difficulty level.
     */
    NumericalAIPlayer(string name, char symbol, AIDifficulty difficulty);
    
    /**
     * @brief Sets the opponent's symbol.
     * @param symbol Opponent's symbol.
     */
    void set_opponent_symbol(char symbol) { opponent_symbol = symbol; }
    
    /**
     * @brief Gets the AI's next move.
     * @param board Current game board.
     * @return Tuple containing (row, col, number).
     */
    tuple<int, int, char> get_best_move(NumericalBoard& board);
    
    /**
     * @brief Gets the AI difficulty level.
     * @return Difficulty level.
     */
    AIDifficulty get_difficulty() const { return difficulty; }
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
private:
    /**
     * @brief Validates if a number choice is valid for the given player.
     * @param number The number chosen.
     * @param player The player making the move.
     * @param board The game board.
     * @return true if valid, false otherwise.
     */
    bool is_valid_number(int number, Player<char>* player, NumericalBoard* board);
    
    /**
     * @brief Prompts user to select AI difficulty.
     * @return Selected difficulty level.
     */
    AIDifficulty select_difficulty();

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
};

#endif // NUMERICAL_TIC_TAC_TOE_H