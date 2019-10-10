#include "../include/player.h"

namespace DMAG {
Player::Player()
{
    this->score = 0;
    this->victory_points = 0;
    this->conflict_tokens = 0;
    this->coins = 3; // every player gets 3 coins at the start of each game
    this->wonder_built = false;
}

void Player::CalculateScore(){
    int _score = this->victory_points + this->conflict_tokens + this->coins;
    //if (wonder_built) this->score += wonder_points // need to figure out how we'll represent wonders before
    //Card _c;
    this->score = _score;
}

int Player::GetScore(){
    return this->score;
}

void Player::Discard(Card c){
    // logic: the card taken at a certain round will be pushed back to the vector,
    //        effectively making it the last one. Therefore, if the player decides do
    //        discard the card he's just taken for three coins, it'll be removed (pop)
    //        from the vector.
    this->cards.pop_back();
    this->coins += 3;
}

void Player::Battle(Player p){
    int current_age_value = 0; // we'll need to get the actual value for this!
    // win condition
    if (this->conflict_tokens > p.conflict_tokens) {
        this->conflict_tokens += current_age_value;
        // Age I   ->  +1 token
        // Age II  ->  +3 tokens
        // Age III ->  +5 tokens
    } else if (this->conflict_tokens < p.conflict_tokens){
        this->conflict_tokens--;
        p.conflict_tokens += current_age_value;
    } // else (equal number of tokens) none are taken
}

}
