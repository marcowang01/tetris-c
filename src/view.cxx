#include "view.hxx"
#include <iostream>
#include <string>
// Convenient type aliases:
using Color = ge211::Color;
static Color const
        board_bg = Color::black(),
        board_color = Color::white(),
        dark = ge211::Color::black(),
        light = {255,105,180},
        I_color = {0, 255, 255},
        L_color = {255, 128, 0},
        rL_color = {0, 51, 204},
        T_color = {102, 0, 204},
        s_color = {0, 255, 0},
        z_color = {255, 0, 0},
        sq_color = {255, 214, 51},
        ball_color = {0,0,0};


using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const grid_size = 20;

View::View(Model const& model)
        : model_(model),
        grid_square_px_(grid_size),
        grid_offset_px_({25,25}),
        light_tile_({tile_size,tile_size}, light),
        dark_tile_({tile_size,tile_size}, dark),
        L_tile_({tile_size,tile_size}, L_color),
        rL_tile_({tile_size,tile_size}, rL_color),
        s_tile_({tile_size,tile_size}, s_color),
        z_tile_({tile_size,tile_size}, z_color),
        T_tile_({tile_size,tile_size}, T_color),
        I_tile_({tile_size,tile_size}, I_color),
        sq_tile_({tile_size,tile_size}, sq_color),
        board_back(initial_window_dimensions(), board_bg),
        board_front({grid_square_px_ - 2, grid_square_px_ - 2},
                    board_color),
        score_tile_1({tile_size,tile_size}, light),
        score_tile_2({tile_size,tile_size}, light),
        ball_sprite_({tile_size, tile_size}, ball_color)
{
    Color ball_c = {0,0,0};
    Player p = model_.ball_.shape_type;
    if (p == Player::L) {
        ball_c = L_color;
    } else if (p == Player::rL){
        ball_c = rL_color;
    } else if (p == Player::s){
        ball_c = s_color;
    } else if (p == Player::sq){
        ball_c = sq_color;
    } else if (p == Player::z){
        ball_c = z_color;
    } else if (p == Player::I){
        ball_c = I_color;
    } else if (p == Player::T){
        ball_c = T_color;
    }
    ball_sprite_ = ge211::Rectangle_sprite({tile_size, tile_size}, ball_c);

}

void View::draw(Sprite_set& set, Position mouse_pos_)
{
    set.add_sprite(board_back, {0,0}, 0);
    Color c_arr[7] = {z_color, L_color, sq_color, s_color, I_color,
                      rL_color, T_color};
    int c_index = (model_.score_ - 1) / 20;
    if (c_index < 0) {
        c_index = 0;
    }

    int score_cap = 20;
    int z1, z2;
    int display_score_1, display_score_2;
    if (model_.score_ % (score_cap * 2) < score_cap){
        z1 = 6;
        z2 = 5;
        display_score_1 = model_.score_ % score_cap;
        if (display_score_1 == 0 && model_.score_ != 0){
            display_score_1 = score_cap;
        }
        if (model_.score_ > (score_cap * 2)){
            display_score_2 = score_cap;
        } else {
            display_score_2 = 0;
        }
    } else {
        z1 = 5;
        z2 = 6;
        display_score_2 = model_.score_  % score_cap;
        if (display_score_2 == 0 && model_.score_ != 0){
            display_score_2 = score_cap;
        }
        display_score_1 = score_cap;
    }


    if (c_index + (z1 - 6) < 0) {
        score_tile_1 = ge211::Rectangle_sprite({tile_size,tile_size},
                                               c_arr[0]);
    } else {
        score_tile_1 = ge211::Rectangle_sprite({tile_size,tile_size},
                                               c_arr[c_index + (z1 - 6)]);
    }
    int height = model_.config_.board_dims.height;
    int width = model_.config_.board_dims.width;
    for (int i = 0; i < display_score_1; i++){
        Position pos = {i % width, (height - 1) - i/width};
        Position screen_pos = board_to_screen_({pos.x, pos.y});
        set.add_sprite(score_tile_1, {screen_pos.x + 1,
                                     screen_pos.y + 1}, z1);
    }

    if (c_index + (z2 - 6) < 0) {
        score_tile_2 = ge211::Rectangle_sprite({tile_size,tile_size},
                                               c_arr[0]);
    } else {
        score_tile_2 = ge211::Rectangle_sprite({tile_size,tile_size},
                                               c_arr[c_index + (z2 - 6)]);
    }

    for (int i = 0; i < display_score_2; i++){
        Position pos = {i % width, (height - 1) - i/width};
        Position screen_pos = board_to_screen_({pos.x, pos.y});
        set.add_sprite(score_tile_2, {screen_pos.x + 1,
                                     screen_pos.y + 1}, z2);
    }

    for (auto pos : model_.board()){
        Player p = model_[pos];
        Position screen_pos = board_to_screen_({pos.x, pos.y});
        set.add_sprite(board_front, {screen_pos.x + 1,
                                     screen_pos.y + 1}, 1);
        screen_pos = {screen_pos.x + tile_offset_px_,
                      screen_pos.y + tile_offset_px_};

        if (pos.y > model_.config_.max_height - 1){
            set.add_sprite(dark_tile_, screen_pos, 4);
        }

        if (p == Player::L) {
            set.add_sprite(L_tile_, screen_pos, 2);
        } else if (p == Player::rL){
            set.add_sprite(rL_tile_, screen_pos, 2);
        } else if (p == Player::s){
            set.add_sprite(s_tile_, screen_pos, 2);
        } else if (p == Player::sq){
            set.add_sprite(sq_tile_, screen_pos, 2);
        } else if (p == Player::z){
            set.add_sprite(z_tile_, screen_pos, 2);
        } else if (p == Player::I){
            set.add_sprite(I_tile_, screen_pos, 2);
        } else if (p == Player::T){
            set.add_sprite(T_tile_, screen_pos, 2);
        } else if (p == Player::light){
            set.add_sprite(light_tile_, screen_pos, 2);
        }
    }

    Position c = ge211::Posn<int>(model_.ball_.center);
    for (ge211::Posn<int> pos : model_.ball_.shape) {
        int dx = pos.x - model_.ball_.shape_center.x;
        int dy = pos.y - model_.ball_.shape_center.y;
        Position temp = {c.x + dx, c.y + dy};

        Position screen_pos = board_to_screen_(temp);
        screen_pos = {screen_pos.x + tile_offset_px_,
                      screen_pos.y + tile_offset_px_};

        Color ball_c = {0,0,0};
        Player p = model_.ball_.shape_type;
        if (p == Player::L) {
            ball_c = L_color;
        } else if (p == Player::rL){
            ball_c = rL_color;
        } else if (p == Player::s){
            ball_c = s_color;
        } else if (p == Player::sq){
            ball_c = sq_color;
        } else if (p == Player::z){
            ball_c = z_color;
        } else if (p == Player::I){
            ball_c = I_color;
        } else if (p == Player::T){
            ball_c = T_color;
        }
        ball_sprite_ = ge211::Rectangle_sprite({tile_size, tile_size}, ball_c);

        set.add_sprite(ball_sprite_,
                       ge211::Posn<int>(screen_pos),2);
    }

    if (model_.is_game_over()) {
        for (auto pos : model_.board()){
            Player p = model_[pos];
            Position screen_pos = board_to_screen_({pos.x, pos.y});
            set.add_sprite(board_front, {screen_pos.x + 1,
                                         screen_pos.y + 1}, 1);
            screen_pos = {screen_pos.x + tile_offset_px_,
                          screen_pos.y + tile_offset_px_};
            if (p != Player::neither) {
                set.add_sprite(light_tile_, screen_pos, 3);
            }
        }
    }


}

// Converts a logical grid position to the physical screen position
// of the upper-left corner of the corresponding grid square.
View::Position
View::board_to_screen_(Position board_pos) const
{
    int x = grid_square_px_ * board_pos.x;
    int y = grid_square_px_ * (model_.board().dimensions().height
            - board_pos.y - 1);

    return Position{x, y} + grid_offset_px_;
}

// Converts a physical screen position to the logical grid position
// that corresponds to it.
View::Position
View::screen_to_board_(Position screen_pos) const
{
    screen_pos -= grid_offset_px_;
    int col_no = screen_pos.x / grid_square_px_;
    int row_no = model_.board().dimensions().height
            - screen_pos.y / grid_square_px_ - 1;
    return {col_no, row_no};
}

View::Dimensions
View::initial_window_dimensions() const
{
    return grid_size * model_.board().dimensions() + grid_offset_px_*2;
}

std::string
View::initial_window_title() const
{
    return "Jank Tetris";
}

