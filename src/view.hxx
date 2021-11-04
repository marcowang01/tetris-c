#pragma once

#include "model.hxx"

#include <string>

class View
{
public:
    /// View dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    /// View positions will use `int` coordinates.
    using Position = ge211::Posn<int>;

    /// View rectangles will use `int` coordinates.
    using Rectangle = ge211::Rect<int>;

    /// Constructs a view that knows about the given model.
    explicit View(Model const&);

    void draw(ge211::Sprite_set& set, Position mouse_pos_);

    Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    /// Helper for converting logical grid positions to physical pixel
    /// positions.
    Position board_to_screen_(Position board_pos) const;

    /// Helper for converting physical pixel positions to logical grid
    /// positions.
    Position screen_to_board_(Position screen_pos) const;

private:


    Model const& model_;

    int tile_offset_px_ = 1;

    int grid_square_px_;

    int tile_size =  grid_square_px_  - tile_offset_px_ * 2;;

    Dimensions grid_offset_px_;

    ge211::Rectangle_sprite const
            light_tile_,dark_tile_,
            L_tile_, rL_tile_, s_tile_, z_tile_,
            T_tile_, I_tile_, sq_tile_,
            board_back, board_front;

    ge211::Rectangle_sprite score_tile_1, score_tile_2, ball_sprite_;
};
