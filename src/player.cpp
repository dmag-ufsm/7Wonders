#include <player.h>
#include <algorithm>

namespace DMAG {
Player::Player()
{
    this->id = 0;
    this->shields = 0;
    this->conflict_tokens = 0;
    this->victory_points = 0;

    this->play_seventh = false;
    this->wonder_raw_cheap = false;
    this->raw_cheap_east = false;
    this->raw_cheap_west = false;
    this->manuf_cheap = false;

    this->player_east = NULL;
    this->player_west = NULL;
}

void Player::BuildWonder(DMAG::Card c){
    // 1) Build Wonder stage if possible.
    // 2) If the stage was built, remove card 'c' from the hand
    //    and insert 'c' to the vector of played cards.
    if (this->board.AddStage(this)) {
        size_t i = 0;
        for (i = 0; i < this->cards_hand.size(); i++) {
            if (this->cards_hand[i].Equal(c)) break;
        }
        this->cards_hand.erase(this->cards_hand.begin()+i);
        this->cards_played.push_back(c);
    }
}

void Player::BuildStructure(DMAG::Card c){
    // TODO: check to see if you can build the structure
}

void Player::BuildGuild(){
    // TODO: check to see if you can build the structure and then play the card
}

std::vector<Card> Player::GetHandCards(){
    return this->cards_hand;
}

void Player::ReceiveCards(std::vector<Card> _cards_hand){
    this->cards_hand = _cards_hand;
}

DMAG::Card Player::Discard(){
    // The card taken at a certain round will be pushed back to the list of played cards,
    // effectively making it the last one. Therefore, if the player decides to discard
    // the card he's just taken for three coins, it'll be removed (pop) from the list.
    this->resources[RESOURCE::coins] += 3;
    Card c = this->cards_played.back();
    this->cards_played.pop_back();
    return c;
}

// This function is a "step" in BuildStructure.
bool Player::BuyResource(int resource, int quant){
    bool is_raw = resource <= 3 ? true : false; // raw materials have code <= 3 in resources.h
    int cost = !is_raw && this->manuf_cheap ? 1*quant : 2*quant;

    Player* east = this->GetEastNeighbor();
    Player* west = this->GetWestNeighbor();

    if (east->HasEnoughResource(resource, quant)) {
        if (is_raw && (this->raw_cheap_east || this->wonder_raw_cheap)) cost = 1*quant;
        if (cost >= this->resources[RESOURCE::coins]) {
            this->resources[RESOURCE::coins] -= cost;
            east->AddResource(RESOURCE::coins, cost);
            return true;
        }
    }
    if (west->HasEnoughResource(resource, quant)) {
        if (is_raw && (this->raw_cheap_west || this->wonder_raw_cheap)) cost = 1*quant;
        if (cost >= this->resources[RESOURCE::coins]) {
            this->resources[RESOURCE::coins] -= cost;
            west->AddResource(RESOURCE::coins, cost);
            return true;
        }
    }

    return false; // couldn't buy
}

void Player::AddResource(int resource, int quant){
    this->resources[resource] += quant;
}

bool Player::HasEnoughResource(int resource, int quant){
    return (this->resources[resource] >= quant);
}

void Player::AddShield(int quant){
    this->shields += quant;
}

void Player::Battle(int age){
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

    // battle with east neighbor
    if (this->shields > player_east->GetShields())
        this->conflict_tokens += current_age_value;
    else if (this->shields < player_east->GetShields())
        this->conflict_tokens -= 1;

    // battle with west neighbor
    if (this->shields > player_west->GetShields())
        this->conflict_tokens += current_age_value;
    else if (this->shields < player_west->GetShields())
        this->conflict_tokens -= 1;
}

int Player::CalculateCivilianScore(){
    // Each Civilian (blue) structure gives a fixed number of VPs.
    int civil_score = 0;

    for (DMAG::Card const& card : this->cards_played) {
        // This will probably need to be modified depending on how Card will be implemented.
        if (card.GetType() == CARD_TYPE::civilian) {
            //civil_score += card.GetValue();
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

// This method must be called every time a scientific card is played by the player to update
// their earned scientific points (also used to see which symbol is most advantageous in
// cases where the player has won a free symbol through the Babylon board or guild card).
int Player::CalculateScientificScore(){
    int gear = this->resources[RESOURCE::gear];
    int tablet = this->resources[RESOURCE::tablet];
    int compass = this->resources[RESOURCE::compass];

    // The smallest value among the three is the amount of completed sets
    int completed_sets = 0;
    if (gear <= tablet && gear <= compass)    completed_sets = gear;
    if (tablet <= gear && tablet <= compass)  completed_sets = tablet;
    if (compass <= gear && compass <= tablet) completed_sets = compass;

    int points_per_set_completed = 7;
    return (gear * gear) + (tablet * tablet) + (compass * compass) + (completed_sets * points_per_set_completed);
}

int Player::CalculateScore(){
    int treasury_score = static_cast<int>(this->resources[RESOURCE::coins]/3); // 1 VP for every 3 coins
    // TODO: all these:
    int civil_score = this->CalculateCivilianScore();
    int commercial_score = this->CalculateCommercialScore();
    int guild_score = this->CalculateGuildScore();
    int science_score = this->CalculateScientificScore();
    this->victory_points += treasury_score + civil_score + commercial_score +
                             guild_score + science_score + this->conflict_tokens;

    if ( this->board.GetStage() > 0) {
        int wonder_score = this->board.GetWonderPoints();
        this->victory_points += wonder_score;
    }

    return this->victory_points;
}

// Called at the end of the game, before scoring.
void Player::ChooseExtraManuf(int resource){
    if (resource == RESOURCE::loom || resource == RESOURCE::glass || resource == RESOURCE::papyrus) {
        if (this->board.GetType() == WONDER_ID::alexandria_b && this->board.GetStage() >= 2) {
            this->resources[resource]++;
        }
    }
}

// Called at the end of the game, before scoring.
void Player::ChooseExtraScience(int resource){
    if (resource == RESOURCE::gear || resource == RESOURCE::compass || resource == RESOURCE::tablet) {
        int id = this->board.GetType();
        int stage = this->board.GetStage();
        if ((id == WONDER_ID::babylon_a && stage >= 2) ||
             (id == WONDER_ID::babylon_b && stage >= 3)) {
            this->resources[resource]++;
        }
    }
}

// Called ONCE each turn. Unsure exactly how this works tbh.
void Player::ChooseExtraRaw(int resource){
    if (resource == RESOURCE::wood || resource == RESOURCE::ore ||
        resource == RESOURCE::clay || resource == RESOURCE::stone) {
        int id = this->board.GetType();
        int stage = this->board.GetStage();
        if ((id == WONDER_ID::alexandria_a && stage >= 2) ||
            (id == WONDER_ID::alexandria_b && stage >= 1)) {
            this->resources[resource]++;
        }
    }
}

// Automatically called when Babylon B stage 2 is constructed.
void Player::CanPlaySeventh(){
    this->play_seventh = true;
}

// Automatically called when Olympia A stage 1 is constructed.
void Player::CanBuyRawCheap(){
    this->wonder_raw_cheap = true;
}

// Will be changed depending on which type discard_pile will be.
// Called at the end of the turn after the stage was built.
void Player::BuildDiscardFree(DMAG::Card c, std::list<DMAG::Card> discard_pile){
    int id = this->board.GetType();
    int stage = this->board.GetStage();
    if ((id == WONDER_ID::halikarnassos_a && stage == 2) ||
        (id == WONDER_ID::halikarnassos_b && stage >= 1)) {
        // 1) Check if card c is in discard_pile.
        // 2) Take c from the discard_pile and push it to cards_played (if it's not there!)
        // 3) Apply card c effects.
    }
}

// Called ONCE per age.
void Player::BuildHandFree(DMAG::Card c){
    if (this->board.GetType() == WONDER_ID::olympia_a && this->board.GetStage() >= 2) {
        // 1) Check if card c is in the hand.
        // 2) Take c from the hand and push it to cards_played (if it's not there!)
        // 3) Apply card c effects.
    }
}

void Player::CopyGuild(DMAG::Card c){
    // :-)
}

int Player::GetShields() {
    return this->shields;
}

std::map<int, unsigned char> Player::GetResources(){
    return this->resources;
}

DMAG::Player* Player::GetEastNeighbor() {
    return this->player_east;
}

DMAG::Player* Player::GetWestNeighbor() {
    return this->player_west;
}

void Player::SetNeighbors(DMAG::Player* east, DMAG::Player* west){
	this->player_east = east;
	this->player_west = west;
}

void Player::SetWonder(DMAG::Wonder _board){
    this->board = _board;
    // TODO: we'll need to add a initial resource on this->resources!
}

void Player::SetId(int id){
    this->id = id;
}

}
