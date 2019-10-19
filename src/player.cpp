#include "../include/player.h"

namespace DMAG {
Player::Player()
{
    this->player_east = NULL;
    this->player_west = NULL;
    this->board = 0;
    this->coins = 3; // every player gets 3 coins at the start of each game
    this->shields = 0;
    this->conflict_tokens = 0;
    this->wonder_stage = 0;
    this->victory_points = 0;
}

void Player::BuildWonder(){
    // TODO: check to see if the player can build a stage of the wonder

    // this->wonder_stage++;
}

void Player::BuildStructure(){
    // TODO: check to see if you can build the structure and then play the card
}

void Player::BuildGuild(){
    // TODO: check to see if you can build the structure and then play the card
}

void Player::ReceiveCards(){
    // TODO
}

DMAG::Card Player::Discard(){
    // The card taken at a certain round will be pushed back to the list of played cards,
    // effectively making it the last one. Therefore, if the player decides to discard
    // the card he's just taken for three coins, it'll be removed (pop) from the list.
    this->coins += 3;
    Card c = this->cards_played.back();
    this->cards_played.pop_back();
    return c;
}

int Player::BuyResource(){
    // TODO: *thinking*
    // This function will be a "step" to BuildStructure!
    // Step 1) Check if the player has commercial structures that give bonuses
    //         (e.g. buying a resource from neighbour for 1 coin instead of 2);
    // Step 2) Check if one of the neighbours has the resource we need;
    // Step 3) Take the shared resource;
    // Step 4) Continue building the structure.
    return 0;
}

int Player::Battle(Player p, int age){
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
        this->conflict_tokens += current_age_value;
        return 1; // 1 -> win
    } else if (this->shields < p.shields) {
        this->conflict_tokens--;
        return -1; // -1 -> lost
    }
    return 0; // 0 -> draw
}

int Player::CalculateCivilianScore(){
    // Each Civilian (blue) structure gives a fixed number of VPs.
    int civil_score = 0;

    for (DMAG::Card const& card : this->cards_played) {
        // This will probably need to be modified depending on how Card will be implemented.
        if (card.GetType() == CARD_TYPE::civilian) {
            civil_score += card.GetValue();
        }
    }

    return civil_score;
}

int Player::CalculateCommercialScore(){
    // Age III commercial structures (yellow) grant victory points based on certain
    // structures a player has built.
    int comm_score = 0;

    // TODO
    // Commerce provides specials so we'll need to check for them.

    return comm_score;
}

int Player::CalculateGuildScore(){
    // The guilds (purple) provide several means of gaining victory points, most of
    // which are based on the types of structure a player and/or his neighbors have built.
    int guild_score = 0;

    // TODO
    // Guilds provide specials so we'll need to check for them.

    return guild_score;
}

// CalculateScientificScore(...) will probably need to be reworked...

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

int Player::CalculateScore(){
    int treasury_score = static_cast<int>(this->coins/3); // 1 VP for every 3 coins
    // TODO: all these:
    // int civil_score = CalculteCivilianScore();
    // int commercial_score = CalculeCommercialScore();
    // int guild_score = CalculateGuildScore();
    // int science_score = CalculateScientificScore();
    // this->victory_points += treasury_score + civil_score + commercial_score +
    //                         guild_score + science_score + this->conflict_tokens;
    if (this->wonder_stage > 0) {
        int wonder_score = 0; // we will need to calculate wonder points based on each stage effect
        this->victory_points += wonder_score;
    }
    return this->victory_points;
}

void Player::SetNeighbours(DMAG::Player *east, DMAG::Player *west){
	this->player_east = east;
	this->player_west = west;
}
}
