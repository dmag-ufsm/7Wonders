#include "../include/player.h"

namespace DMAG {
Player::Player()
{
    this->score = 0;
    this->victory_points = 0;
}

void Player::CalculateScore(){
    int _score = 0;
    Card _c;
    do{
        _c = _deck.GetTop();
        _score += _c.GetValue();
    }while(!_deck.IsEmpty());
    this->score = _score;
}

int Player::GetScore(){
    return this->score;
}

}
