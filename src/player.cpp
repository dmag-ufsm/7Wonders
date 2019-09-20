#include <player.h>

namespace DMAG {
Player::Player()
{
    this->score = 0;
    this->victory_points = 0;
}

void Player::CalculateScore(){
    this->score = this->_deck.GetPoints();
}

int Player::GetScore(){
    this->CalculateScore();
    return this->score;
}

void Player::ReceiveCard(Card _c) {
    this->_deck.Insert(_c);
}

void Player::ReceiveCards(std::list<Card> _c) {
    for(std::list<Card>::iterator it = _c.begin(); it != _c.end(); ++it){
        this->ReceiveCard(*it);
    }

}

}
