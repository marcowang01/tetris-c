#include "game_config.hxx"

// This is the default (and only) constructor for `Game_config`. It
// determines all the default values of all the member variables.
Game_config::Game_config()
        :   board_dims{10, 25},
            ball_radius{18},
            ball_velocity_0{0, -1},
            ball_start_center{5,20},
            max_height{20},
            max_score{140}
{ }
