
#include "player.hxx"

std::ostream& operator<<(std::ostream& os, Player p)
{
    switch (p) {
    case Player::light:
        return os << "L";
    case Player::sq:
        return os << "q";
    case Player::L:
        return os << "L";
    case Player::rL:
        return os << "r";
    case Player::s:
        return os << "s";
    case Player::z:
        return os << "z";
    case Player::T:
        return os << "T";
    case Player::I:
        return os << "I";
    default:
        return os << "_";
    }
}

