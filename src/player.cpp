#include "../include/player.h"
#include <list>

namespace DMAG {
Player::Player()
{
    this->score = 0;
    this->victory_points = 0;
    this->victory_tokens = 0;
    this->defeat_tokens = 0;
    this->coins = 3; // every player gets 3 coins at the start of each game
    this->shields = 0;
    this->wonder_built = false;
}


void Player::GiveCards(){
}

void Player::CalculateScore(){
    int _score = (this->victory_points + this->victory_tokens + this->coins) - this->defeat_tokens;
    if (wonder_built) {
        int wonder_points = 0; // need to figure out how we'll represent wonders to calculate this!
        _score += wonder_points;
    }
    this->score = _score;
}

// this method must be called every time a scientific card is played by the player to update
// their earned scientific points (also used to see which symbol is most advantageous in
// cases where the player has won a free symbol through the Babylon board or guild card)
int Player::CalculateScientificScore(int gear, int tablet, int compass){
    // the smallest value among the three is the amount of completed sets
    int completed_sets = 0;
    if (gear <= tablet && gear <= compass)    completed_sets = gear;
    if (tablet <= gear && tablet <= compass)  completed_sets = tablet;
    if (compass <= gear && compass <= tablet) completed_sets = compass;

    int points_per_set_completed = 7;
    return (gear * gear) + (tablet * tablet) + (compass * compass) + (completed_sets * points_per_set_completed);
}

int Player::GetScore(){
    return this->score;
}

void Player::Discard(Card c){
    // logic: the card taken at a certain round will be pushed back to the vector,
    //        effectively making it the last one. Therefore, if the player decides do
    //        discard the card he's just taken for three coins, it'll be removed (pop)
    //        from the vector.
    // this'll probably need to be reworked
    this->cards_played.pop_back();
    this->coins += 3;
    // todo: put discarded card into the discard deck
}

void Player::Battle(Player p, int age){
    int current_age_value = 1;

    // Age I   ->  +1 victory token
    // Age II  ->  +3 victory tokens
    // Age III ->  +5 victory tokens
    switch (age) {
        case 2:
            current_age_value = 3;
            break;
        case 3:
            current_age_value = 5;
            break;
        default:
            break;
    }

    if (this->shields > p.shields) {
        this->victory_tokens += current_age_value;
    } else if (this->shields < p.shields) {
        this->defeat_tokens++;
        p.victory_tokens += current_age_value;
    } // else (equal number of shields) none are taken
}

}
