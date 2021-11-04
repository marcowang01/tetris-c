
// Defines a struct to hold configuration parameters. You should try different
// values to ensure that your model and UI adjust appropriately. You can do this
// by creating an instance and then assigning its parameters before constructing
// a `Model`:


#pragma once

#include <ge211.hxx>


struct Game_config
{
    // Constructs an instance with the default parameters.
    Game_config();

    // The dimensions of the whole window:
    ge211::Dims<int> board_dims;

    // The radius of the ball:
    int ball_radius;

    // The ball's initial velocity {width, height}:
    ge211::Dims<int> ball_velocity_0;

    ge211::Posn<int> ball_start_center;

    uint max_height;
    uint max_score;

};

