#pragma once

#include "model.hxx"
#include "view.hxx"

// The main game class for Reversi.
class Controller : public ge211::Abstract_game
{
public:


    Controller();

// `protected` means that GE211 (via base class `ge211::Abstract_game`)
// can access these members, but arbitrary other code cannot:
protected:
    //
    // Controller operations called by GE211
    //


    // These three delegate to the view:
    void draw(ge211::Sprite_set&) override;
    View::Dimensions initial_window_dimensions() const override;
    std::string initial_window_title() const override;

    void on_mouse_move(View::Position mouse_position) override;

    void on_key(ge211::Key) override;

    void on_mouse_down(
            ge211::Mouse_button button,
            View::Position mouse_position) override;

    void on_frame(double dt) override;

private:
    Model            model_;
    View             view_;

    View::Position mouse_pos_ = {-1, -1};
};
