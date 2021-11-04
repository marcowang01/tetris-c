/*************************************************/
/*** DO NOT CHANGE THE PUBLIC API IN THIS FILE ***/
/*************************************************/
//
// You may add private members if you like, or even remove
// the private helper functions, but you mustn't change the
// existing public members, nor add new ones. If you do, your
// code may not build for automated tests.

#pragma once

#include "player.hxx"
#include "board.hxx"
#include "ball.hxx"
#include "game_config.hxx"

#include <ge211.hxx>

#include <iostream>
#include <vector>

// Represents the state of the Reversi game.
class Model
{
public:

    /// Model dimensions will use `int` coordinates, as board dimensions do.
    using Dimensions = Board::Dimensions;

    /// Model positions will use `int` coordinates, as board positions do.
    using Position = Board::Position;

    /// Model rectangles will use `int` coordinates, as board rectangles do.
    using Rectangle = Board::Rectangle;


    /// Constructs a model with the given width and height.
    ///
    /// ## Errors
    ///
    ///  - Throws `ge211::Client_logic_error` if either dimension is less
    ///    than 2 or greater than 8.
    Model(Game_config const& config = Game_config());

    /// Returns a rectangle containing all the positions of the board.
    /// This can be used to iterate over the positions.
    Rectangle board() const;

    /// Returns whether the game is finished. This is true when neither
    /// player can move.
    bool is_game_over() const
    { return game_over_; }


    /// Returns the player at the given position, or `Player::neither` if
    /// the position is unoccupied.
    ///
    /// ## Errors
    ///
    ///  - Throws `ge211::Client_logic_error` if the position is out of
    ///    bounds.
    Player operator[](Position) const;

    void on_frame(double dt);

    void rotate();

    void translate(std::string dir);

    void drop();

    void clear_lines();

    void ball_falls();

    void restart_game();

    Game_config const config_;
    Ball ball_;
    double place_delay_;
    double key_cooldown_;
    int score_;
    bool I_mode_;
    bool T_mode_;

#ifdef CS211_TESTING
    // When this class is compiled for testing, members of a struct named
    // Test_access will be allowed to access private members of this class.
    friend struct Test_access;
#endif

private:

    Board board_;
    Position_set pset_;
    bool game_over_;
};

