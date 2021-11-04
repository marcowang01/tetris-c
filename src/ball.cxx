// YOU DEFINITELY NEED TO MODIFY THIS FILE.

#include "ball.hxx"
#include "player.hxx"
#include "game_config.hxx"

Ball::Ball(Game_config const& config, std::string type)
        : radius(config.ball_radius),
          center(config.ball_start_center),
          velocity(Velocity(config.ball_velocity_0)),
          live(true),
          time(0.0),
          shape(),
          shape_center(0,0),
          shape_type(Player::neither),
          max_height(config.max_height),
          board_dims(config.board_dims)
{
    if (type.compare("L") == 0){
        shape[{10,9}] = true;
        shape[{10,11}] = true;
        shape[{11,9}] = true;
        shape[{10,10}] = true;
        shape_center = {10,10};
        shape_type = Player::L;
    } else if (type.compare("rL") == 0){
        shape[{10,10}] = true;
        shape[{10,9}] = true;
        shape[{10,11}] = true;
        shape[{9,9}] = true;
        shape_center = {10,10};
        shape_type = Player::rL;
    } else if (type.compare("sq") == 0){
        shape[{0,0}] = true;
        shape[{1,0}] = true;
        shape[{1,1}] = true;
        shape[{0,1}] = true;
        shape_center = {1,0};
        shape_type = Player::sq;
    } else if (type.compare("I") == 0){
        shape[{19,21}] = true;
        shape[{20,21}] = true;
        shape[{21,21}] = true;
        shape[{22,21}] = true;
        shape_center = {21,21};
        shape_type = Player::I;
    } else if (type.compare("T") == 0){
        shape[{10,11}] = true;
        shape[{11,11}] = true;
        shape[{12,11}] = true;
        shape[{11,12}] = true;
        shape_center = {11,11};
        shape_type = Player::T;
    } else if (type.compare("s") == 0) {
        shape[{10,11}] = true;
        shape[{11,11}] = true;
        shape[{11,12}] = true;
        shape[{12,12}] = true;
        shape_center = {11,11};
        shape_type = Player::s;
    } else if (type.compare("z") == 0) {
        shape[{12,11}] = true;
        shape[{11,11}] = true;
        shape[{11,12}] = true;
        shape[{10,12}] = true;
        shape_center = {11,11};
        shape_type = Player::z;
    }
}


int
Ball::hits_bottom() const
{
    int num = 0;
    for (ge211::Posn<int> pos : shape) {
        int dy = pos.y - shape_center.y;

        if (center.y + dy < 0){
            if (center.y + dy < num){
                num = center.y + dy;
            }
        }
    }

    return num * -1;
}


int
Ball::hits_top() const
{
    int num = 0;
    for (ge211::Posn<int> pos : shape) {
        int dy = pos.y - shape_center.y;

        if (center.y + dy > max_height - 1){
            if (center.y + dy > num){
                num = center.y + dy - (max_height - 1);
            }
        }
    }

    return num;
}

int
Ball::hits_side() const
{
    int num = 0;
    for (ge211::Posn<int> pos : shape) {
        int dx = pos.x - shape_center.x;

        if (center.x + dx > (board_dims.width - 1)){
            if (center.x + dx > num){
                num = center.x + dx - (board_dims.width - 1);
            }
        } else if (center.x + dx < 0){
            if (center.x + dx < num){
                num = center.x + dx;
            }
        }
    }

    return num * -1;
}

Ball
Ball::next(double dt, double delay) const
{
    Ball result(*this);

    result.time += dt;
    if (result.time >= delay){
        result.center.y += velocity.height;
        result.time = 0;
    }

    return result;
}

Ball
Ball::rotate(Position_set pset) const
{
    Ball result(*this);
    Position_set temp = Position_set();
    Position_set original = result.shape;
    if (result.shape_center.x != 1){
        for (ge211::Posn<int> pos : result.shape) {

            Position vr = {pos.x - result.shape_center.x,
                           pos.y - result.shape_center.y};
            Position vt = {pos.y - result.shape_center.y,
                           - pos.x + result.shape_center.x};
            Position vp = {vt.x + result.shape_center.x,
                           vt.y + result.shape_center.y};
            temp[{int(vp.x), int(vp.y)}] = true;
        }
        result.shape = temp;
        if (result.hits_block(pset)) {
            result.shape = original;
        }
    }

    return result;
}

Ball
Ball::translate(std::string dir, Position_set pset) const
{
    Ball result(*this);

    if (dir.compare("Left")){
        result.center.x += 1;
        if (result.hits_block(pset)){
            result.center.x -= 1;
        }
    } else if (dir.compare("Right")){
        result.center.x -= 1;
        if (result.hits_block(pset)){
            result.center.x += 1;
        }
    }

    return result;
}

Ball
Ball::drop(Position_set pset) const
{
    Ball result(*this);

    result.center.y -= 1;
    if (result.hits_block(pset) || result.hits_bottom() != 0){
        result.center.y += 1;
    }

    return result;
}


bool
Ball::hits_block(Position_set pset) const
{
    Position p = {0,0};
    for (ge211::Posn<int> pos : shape) {
        for (ge211::Posn<int> filled_pos : pset) {
            int dx = pos.x -  shape_center.x;
            int dy = pos.y - shape_center.y;
            Position temp = {center.x + dx, center.y + dy};
            if (ge211::Posn<int>(temp) == filled_pos){
                return true;
            }
        }
    }

    return  false;
}



bool
operator==(Ball const& a, Ball const& b)
{
    return a.radius == b.radius && a.center == b.center
           && a.velocity == b.velocity && a.live == b.live;
}

bool
operator!=(Ball const& a, Ball const& b)
{
    return !(a == b);
}

std::ostream&
operator<<(std::ostream& o, Ball const& ball)
{
    o << "Ball{";
    o << " x: " << ball.center.x << " | y: " << ball.center.y ;
    o << " | Vx: " << ball.velocity.width << " | Vy: " << ball.velocity.height;
    o << " | radius: " << ball.radius;
    return o << "}";
}
