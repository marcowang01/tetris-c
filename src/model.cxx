#include "model.hxx"
#include <iostream>
#include <vector>

using namespace ge211;

Model::Model(Game_config const& config)
        : config_(config),
          ball_(config, " "),
          place_delay_(0.0),
          key_cooldown_(0.0),
          score_(0),
          I_mode_(false),
          T_mode_(false),
          board_(config.board_dims),
          pset_(),
          game_over_(true)
{ }

Model::Rectangle Model::board() const
{
    return board_.all_positions();
}

Player Model::operator[](Position pos) const
{
    return board_[pos];
}

void Model::rotate(){
    if (key_cooldown_ == 0){
        ball_ = ball_.rotate(pset_);
        if (ball_.hits_block(pset_)){
            ball_ = ball_.rotate(pset_);
            ball_ = ball_.rotate(pset_);
            ball_ = ball_.rotate(pset_);
        }
        else if (!ball_.hits_block(pset_) && ball_.hits_side() != 0){
            int dx = ball_.hits_side();
            ball_.center.x += dx;

            if (ball_.hits_block(pset_)){
                ball_.center.x -= dx;
                ball_ = ball_.rotate(pset_);
                ball_ = ball_.rotate(pset_);
                ball_ = ball_.rotate(pset_);

            }
        } else if (ball_.hits_block(pset_) && ball_.hits_side() != 0){
            ball_ = ball_.rotate(pset_);
            ball_ = ball_.rotate(pset_);
            ball_ = ball_.rotate(pset_);
        }

        key_cooldown_ += 1;
    }
}

void Model::translate(std::string dir){

    if (key_cooldown_ == 0){
        ball_ = ball_.translate(dir, pset_);
        if (ball_.hits_block(pset_) || ball_.hits_side() != 0){
            if (dir.compare("Left") == 0){
                ball_ = ball_.translate("Right", pset_);
            } else {
                ball_ = ball_.translate("Left", pset_);
            }
        }

        key_cooldown_ += 1;
    }
}

void Model::drop(){

    if (key_cooldown_ == 0){
        ball_ = ball_.drop(pset_);
        if (ball_.hits_block(pset_) || ball_.hits_bottom() != 0){
            ball_.center.y += 1;
        }

        key_cooldown_ += 1;
    }
}


void Model::clear_lines(){

    int tiles_per_row[24] = {0};

    for (ge211::Posn<int> pos : pset_) {
        tiles_per_row[pos.y] += 1;
    }

    int num_of_rows = 0;
    std::vector<int> rows_cleared;
    for (int j = 0; j < config_.max_height; j++){
        if (tiles_per_row[j] == 10){
            for (int i = 0; i < 10; i++){
                pset_[{i,j}] = false;
                board_[{i,j}] = Player::neither;
            }
            num_of_rows++;
            rows_cleared.push_back(j);
        }
    }
    if (score_ + num_of_rows > config_.max_score){
        game_over_ = true;
        score_ = config_.max_score;
    } else {
        score_ += num_of_rows;
    }
    if (num_of_rows > 0){
        Position_set temp_pset = Position_set();
        Board temp_board = Board(board_.dimensions());
        for (ge211::Posn<int> pos : pset_) {
            Position new_p = pos;
            for (int i = 0; i < rows_cleared.size(); i++){
                if (pos.y > rows_cleared[i]){
                    new_p = {pos.x, pos.y - (i + 1)};
                }
            }

            temp_pset[new_p] = true;
            temp_board[new_p] = board_[pos];
        }
        pset_= temp_pset;
        board_ = temp_board;
    }
}

void
Model::ball_falls()
{
    double short_delay = 0.001;
    if (ball_.hits_top() < 2){
        while (!ball_.next(1, short_delay).hits_block(pset_) &&
               ball_.next(1, short_delay).hits_bottom() == 0){
            ball_ = ball_.next(1, 0.001);
        }
        place_delay_  = 10;
        ball_.live = false;
    }
}

void
Model::restart_game()
{
    if (game_over_){
        pset_.clear();
        board_ = Board(board_.dimensions());

        place_delay_ = 0.0;
        key_cooldown_ = 0.0;
        score_ = 0;
        game_over_ = false;

        std::string shapes[7] = {"L", "rL", "sq", "s", "z", "T", "I"};
        ge211::Random_source<int> rand_num(0, 6);
        int r_num = rand_num.next();
        if (I_mode_){
            ball_ = Ball(config_, shapes[6]);
        } else if (T_mode_){
            ball_ = Ball(config_, shapes[5]);
        } else {
            ball_ = Ball(config_, shapes[r_num]);
        }
    }
}


void
Model::on_frame(double dt)
{
    if (score_ >= config_.max_score){
        game_over_ = true;
        ball_.live = false;
    }
    key_cooldown_ = 0;

    // check if 1 second has passed
    if (place_delay_ > 1){
        place_delay_ = 0;
        double short_delay = 0.001;
        if (ball_.next(dt, short_delay).hits_block(pset_) ||
            ball_.next(dt, short_delay).hits_bottom() > 0){

            // place ball
            Position c = ge211::Posn<int>(ball_.center);
            for (ge211::Posn<int> pos : ball_.shape) {
                int dx = pos.x - ball_.shape_center.x;
                int dy = pos.y - ball_.shape_center.y;
                pset_[{c.x + dx, c.y + dy}] = true;
                board_[{c.x + dx, c.y + dy}] = ball_.shape_type;
                if (c.y + dy >= config_.max_height){
                    game_over_ = true;
                }
            }
            clear_lines();
            if (game_over_){
                ball_.live = false;
                return;
            }

            // generate new random ball
            std::string shapes[7] = {"L", "rL", "sq", "s", "z", "T", "I"};
            ge211::Random_source<int> rand_num(0, 6);
            int r_num = rand_num.next();
            if (I_mode_){
                ball_ = Ball(config_, shapes[6]);
            } else if (T_mode_){
                ball_ = Ball(config_, shapes[5]);
            } else {
                ball_ = Ball(config_, shapes[r_num]);
            }
        } else if (!game_over_){
            ball_.live = true;
        }
    }

    // start 1 second delay for ball
    if (!ball_.live){
        place_delay_ += dt;
    }
    // score/speed control
    int level = score_ / 10;
    double delay_factor[15] = {1, 0.8, 0.6, 0.5, 0.4, 0.3, 0.2, 0.15,
                               0.1, 0.08, 0.06, 0.04, 0.02, 0.01, 0.01};
    double delay = delay_factor[level];
    Ball next_b = ball_.next(dt,delay);
    if (next_b.hits_block(pset_) && ball_.hits_top() != 0){
        game_over_ = true;
        ball_.live = false;
        place_delay_ = 10;
        return;
    }

    if (next_b.hits_bottom() > 0){
        ball_.center.y += next_b.hits_bottom();
        ball_.live = false;
    }

    if (next_b.hits_block(pset_)){
        if(!ball_.live){
            ball_.center.y -= next_b.hits_bottom();
        }
        ball_.live = false;
        return;
    }
    ball_ = ball_.next(dt,delay);
}



