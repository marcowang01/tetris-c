#include "controller.hxx"

Controller::Controller()
        : model_(),
          view_(model_)
{ }

void
Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites, mouse_pos_);
}

void
Controller::on_key(ge211::Key key)
{
    if (key == ge211::Key::up()) {
        if (model_.ball_.live && !model_.is_game_over()){
            model_.rotate();
        }
    }

    if (key == ge211::Key::down()) {
        if (model_.ball_.live && !model_.is_game_over()) {
            model_.drop();
        }
    }

    if (key == ge211::Key::left()) {
        if (model_.ball_.live && !model_.is_game_over()) {
            model_.translate("Left");
        }
    }

    if (key == ge211::Key::right()) {
        if (model_.ball_.live && !model_.is_game_over()) {
            model_.translate("Right");
        }
    }

    if (key == ge211::Key::code(' ')) {
        if (model_.ball_.live && !model_.is_game_over()) {
            model_.ball_falls();
        }
    }

    if (key == ge211::Key::code('r')) {
        model_.restart_game();
    }

    if (key == ge211::Key::code('1')) {
        if (model_.score_ + 1 > model_.config_.max_score){
            model_.score_ = model_.config_.max_score;
        } else {
            model_.score_ += 1;
        }
    }

    if (key == ge211::Key::code('2')) {
        if (model_.score_ + 10 > model_.config_.max_score){
            model_.score_ = model_.config_.max_score;
        } else {
            model_.score_ += 10;
        }
    }

    if (key == ge211::Key::code('i')) {
        model_.I_mode_ = !model_.I_mode_;
        model_.T_mode_ = false;
    }

    if (key == ge211::Key::code('t')) {
        model_.T_mode_ = !model_.T_mode_;
    }

}

void
Controller::on_mouse_down(
        ge211::Mouse_button btn,
        View::Position screen_posn)
{
    // if (model_.turn() == Player::neither) return;
    // if (btn != ge211::Mouse_button::left) return;
    //
    // View::Position pos = view_.screen_to_board_(screen_posn);
    //
    // if (model_.find_move(pos)) {
    //     model_.play_move(pos);
    // }
}

void
Controller::on_mouse_move(View::Position screen_pos)
{
    mouse_pos_ = view_.screen_to_board_(screen_pos);
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

void
Controller::on_frame(double dt)
{
    model_.on_frame(dt);
}
