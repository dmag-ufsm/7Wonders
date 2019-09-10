#include "../include/player.h"

namespace DMAG {
Player::Player()
{
    this->score = 0;
}

void Player::CalculateScore(){
    return;
}

int Player::GetScore(){
    return this->score;
}

}
