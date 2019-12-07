#include <player.h>
#include <algorithm>

namespace DMAG {
Player::Player()
{
    this->id = 0;
    this->victory_tokens = 0;
    this->defeat_tokens = 0;
    this->victory_points = 0;

    this->play_seventh = false;
    this->wonder_raw_cheap = false;
    this->raw_cheap_east = false;
    this->raw_cheap_west = false;
    this->manuf_cheap = false;

    this->raw_extra = 0;
    this->manuf_extra = 0;

    this->player_east = NULL;
    this->player_west = NULL;
}

//////////////////
// CARD-RELATED //
//////////////////

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

void Player::BuildStructure(DMAG::Card c, std::vector<DMAG::Card> cards, bool _free_card){
    // returns if the card has already been played (cannot play the same card twice)
    //if (std::find(cards_played.begin(), cards_played.end(), c) != cards_played.end())
    //    return;

    bool free_card = _free_card;

    if (!free_card) {
        // if there are not enough resources
        if (!c.CanBePlayed(this->resources)) {
            // TODO: check if the card can be played for free, otherwise check buy from neighbors.
            // free_card will be manipulated here if it was initially false ("normal" build).
            // this->BuyResource()...

            return;
        }
    }

    int cost = 0;
    int card = c.GetId();
    switch (c.GetType()) {
        case CARD_TYPE::materials:
            if (card == CARD_ID::lumber_yard) {
                this->resources[RESOURCE::wood] += 1;
            } else if (card == CARD_ID::stone_pit) {
                this->resources[RESOURCE::stone] += 1;
            } else if (card == CARD_ID::clay_pool) {
                this->resources[RESOURCE::clay] += 1;
            } else if (card == CARD_ID::ore_vein) {
                this->resources[RESOURCE::ore] += 1;
            } else if (card == CARD_ID::tree_farm) {
                // wood or clay (check how this will be implemented)
                cost = 1;
            } else if (card == CARD_ID::excavation) {
                // stone or clay (check how this will be implemented)
                cost = 1;
            } else if (card == CARD_ID::clay_pit) {
                // clay or ore (check how this will be implemented)
                cost = 1;
            } else if (card == CARD_ID::timber_yard) {
                // stone or wood (check how this will be implemented)
                cost = 1;
            } else if (card == CARD_ID::forest_cave) {
                // wood or ore (check how this will be implemented)
                cost = 1;
            } else if (card == CARD_ID::mine) {
                // ore or stone (check how this will be implemented)
                cost = 1;
            } else if (card == CARD_ID::sawmill) {
                this->resources[RESOURCE::wood] += 2;
                cost = 1;
            } else if (card == CARD_ID::quarry) {
                this->resources[RESOURCE::stone] += 2;
                cost = 1;
            } else if (card == CARD_ID::brickyard) {
                this->resources[RESOURCE::clay] += 2;
                cost = 1;
            } else if (card == CARD_ID::foundry) {
                this->resources[RESOURCE::ore] += 2;
                cost = 1;
            }
            break;

        case CARD_TYPE::manufactured:
            if (card == CARD_ID::loom) {
                this->resources[RESOURCE::loom]++;
            } else if (card == CARD_ID::glassworks) {
                this->resources[RESOURCE::glass]++;
            } else if (card == CARD_ID::press) {
                this->resources[RESOURCE::papyrus]++;
            }
            break;

        case CARD_TYPE::civilian:
            // nothing to do (the VP of these cards are calculated at the end of the game)
            break;

        case CARD_TYPE::commercial:
            if (card == CARD_ID::tavern) {
                this->resources[RESOURCE::coins] += 5;
            } else if (card == CARD_ID::east_trading_post) {
                this->raw_cheap_east = true;
            } else if (card == CARD_ID::west_trading_post) {
                this->raw_cheap_west = true;
            } else if (card == CARD_ID::marketplace) {
                this->manuf_cheap = true;
            } else if (card == CARD_ID::forum) {
                this->manuf_extra++;
            } else if (card == CARD_ID::caravansery) {
                this->raw_extra++;
            } else if (card == CARD_ID::vineyard) {
                int brown_cards = player_east->AmountOfType(CARD_TYPE::materials)
                                + player_west->AmountOfType(CARD_TYPE::materials)
                                + this->AmountOfType(CARD_TYPE::materials);
                this->resources[RESOURCE::coins] += 1 * brown_cards;
            } else if (card == CARD_ID::bazar) {
                int grey_cards = player_east->AmountOfType(CARD_TYPE::manufactured)
                                + player_west->AmountOfType(CARD_TYPE::manufactured)
                                + this->AmountOfType(CARD_TYPE::manufactured);
                this->resources[RESOURCE::coins] += 2 * grey_cards;
            } else if (card == CARD_ID::haven) {
                int brown_cards = this->AmountOfType(CARD_TYPE::materials);
                this->resources[RESOURCE::coins] += 1 * brown_cards;
            } else if (card == CARD_ID::lighthouse) {
                int yellow_cards = this->AmountOfType(CARD_TYPE::commercial);
                this->resources[RESOURCE::coins] += 1 * yellow_cards;
            } else if (card == CARD_ID::chamber_of_commerce) {
                int grey_cards = this->AmountOfType(CARD_TYPE::manufactured);
                this->resources[RESOURCE::coins] += 2 * grey_cards;
            } else if (card == CARD_ID::arena) {
                int stages = this->board.GetStage();
                this->resources[RESOURCE::coins] += 3 * stages;
            }
            break;

        case CARD_TYPE::military:
            if (card == CARD_ID::stockade || card == CARD_ID::barracks || card == CARD_ID::guard_tower) {
                this->resources[RESOURCE::shields] += 1;

            } else if (card == CARD_ID::walls   || card == CARD_ID::training_ground ||
                       card == CARD_ID::stables || card == CARD_ID::archery_range) {
                this->resources[RESOURCE::shields] += 2;

            } else if (card == CARD_ID::fortifications || card == CARD_ID::circus ||
                       card == CARD_ID::arsenal        || card == CARD_ID::siege_workshop) {
                this->resources[RESOURCE::shields] += 3;
            }
            break;

        case CARD_TYPE::scientific:
            // nothing to do (the pieces and VP are calculated at the end of the game)
            break;

        case CARD_TYPE::guild:
            // nothing to do (the VP of these cards are calculated at the end of the game)
            break;
    }

    size_t i = 0;
    for (i = 0; i < cards.size(); i++) {
        if (cards[i].Equal(c)) break;
    }
    if (i == cards.size()) {
        // ERROR: Card played was not found in the vector
    } else {
        cards.erase(cards.begin()+i);
        cards_played.push_back(c);
    }

    if (!free_card) this->resources[RESOURCE::coins] -= cost;
}

std::vector<Card> Player::GetHandCards(){
    return this->cards_hand;
}

void Player::ReceiveCards(std::vector<Card> _cards_hand){
    this->cards_hand = _cards_hand;
}

void Player::Discard(){
	// This just adds some coins to the player total.
    this->resources[RESOURCE::coins] += 3;
}

// Returns the quantity of played cards of a given type (commercial, military, materials, etc.)
int Player::AmountOfType(int card_type){
    int quant = 0;
    for (DMAG::Card const& card : this->cards_played) {
        if (card.GetType() == card_type) {
            quant++;
        }
    }
    return quant;
}


//////////////////////
// RESOURCE-RELATED //
//////////////////////

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

bool Player::CanPlayFree(DMAG::Card c) {
    for (DMAG::Card const&  card : this->cards_played) {
        if (card.GetFreeWith() == card.GetId())
            return true;
    }
    return false;

    // TODO: fix the only case of the Forum card that makes it
    // possible to be free from two others (may need to use vector)
}


////////////////////
// BATTLE-RELATED //
////////////////////

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

    int this_shields = this->resources[RESOURCE::shields];

    // battle with east neighbor
    if (this_shields > player_east->GetShields())
        this->victory_tokens += current_age_value;
    else if (this_shields < player_east->GetShields())
        this->defeat_tokens += 1;

    // battle with west neighbor
    if (this_shields > player_west->GetShields())
        this->victory_tokens += current_age_value;
    else if (this_shields < player_west->GetShields())
        this->defeat_tokens += 1;
}


/////////////////////
// SCORING-RELATED //
/////////////////////

// Calculates the number of VPs the player gets from civilian structures.
int Player::CalculateCivilianScore(){
    // Each Civilian (blue) structure gives a fixed number of VPs.
    int civil_score = 0;

    for (DMAG::Card const& card : this->cards_played) {
        if (card.GetType() != CARD_TYPE::civilian)
            continue;

        switch (card.GetId()) {
            case CARD_ID::altar:
            case CARD_ID::theater:
                civil_score += 2;
                break;

            case CARD_ID::pawnshop:
            case CARD_ID::baths:
            case CARD_ID::temple:
                civil_score += 3;
                break;

            case CARD_ID::courthouse:
            case CARD_ID::statue:
                civil_score += 4;
                break;

            case CARD_ID::aqueduct:
            case CARD_ID::gardens:
                civil_score += 5;
                break;

            case CARD_ID::town_hall:
            case CARD_ID::senate:
                civil_score += 6;
                break;

            case CARD_ID::pantheon:
                civil_score += 7;
                break;

            case CARD_ID::palace:
                civil_score += 8;
                break;
        }
    }

    return civil_score;
}

// Calculates the number of VPs the player gets from certain commercial structures.
// (the other special effects will be calculated when the structure is built!)
int Player::CalculateCommercialScore(){
    int comm_score = 0;

    for (DMAG::Card const& card : this->cards_played) {
        if (card.GetType() != CARD_TYPE::commercial)
            continue;

        switch (card.GetId()) {
            case (CARD_ID::haven):
                // +1 VP per Raw Material CARD in your own city.
                comm_score += this->AmountOfType(CARD_TYPE::materials);
                break;

            case (CARD_ID::lighthouse):
                // +1 VP per Commercial Structure CARD in your own city.
                comm_score += this->AmountOfType(CARD_TYPE::commercial);
                break;

            case (CARD_ID::chamber_of_commerce):
                // +2 VP per Manufactured Good CARD in your own city.
                comm_score += (this->AmountOfType(CARD_TYPE::manufactured) * 2);
                break;

            case (CARD_ID::arena):
                // +1 VP Coin per Wonder stage constructed in your own city.
                comm_score += this->board.GetStage();
                break;
        }
    }

    return comm_score;
}

// Calculates the number of VPs the player gets from guild structures.
int Player::CalculateGuildScore(){
    // The guilds (purple) provide several means of gaining victory points, most of
    // which are based on the types of structure a player and/or his neighbors have built.
    int guild_score = 0;

    for (DMAG::Card const& card : this->cards_played) {
        if (card.GetType() != CARD_TYPE::guild)
            continue;

        switch (card.GetId()) {
            case CARD_ID::workers:
                guild_score = player_east->AmountOfType(CARD_TYPE::materials)
                            + player_west->AmountOfType(CARD_TYPE::materials);
                break;

            case CARD_ID::craftsmens:
                guild_score = 2 * (player_east->AmountOfType(CARD_TYPE::manufactured)
                                + player_west->AmountOfType(CARD_TYPE::manufactured));
                break;

            case CARD_ID::traders:
                guild_score = player_east->AmountOfType(CARD_TYPE::commercial)
                            + player_west->AmountOfType(CARD_TYPE::commercial);
                break;

            case CARD_ID::philosophers:
                guild_score = player_east->AmountOfType(CARD_TYPE::scientific)
                            + player_west->AmountOfType(CARD_TYPE::scientific);
                break;

            case CARD_ID::spies:
                guild_score = player_east->AmountOfType(CARD_TYPE::military)
                            + player_west->AmountOfType(CARD_TYPE::military);
                break;

            case CARD_ID::magistrates:
                guild_score = player_east->AmountOfType(CARD_TYPE::civilian)
                            + player_west->AmountOfType(CARD_TYPE::civilian);
                break;

            case CARD_ID::shipowners:
                guild_score = this->AmountOfType(CARD_TYPE::materials)
                            + this->AmountOfType(CARD_TYPE::manufactured)
                            + this->AmountOfType(CARD_TYPE::guild);
                break;

            case CARD_ID::strategists:
                guild_score = player_east->GetDefeatTokens()
                            + player_west->GetDefeatTokens();
                break;

            case CARD_ID::scientists:
                // computed in CalculateScientificScore()
                break;

            case CARD_ID::builders:
                guild_score = player_east->GetBoard().GetStage()
                            + player_west->GetBoard().GetStage()
                            + this->board.GetStage();
                break;
        }
    }

    return guild_score;
}

int Player::CalculateScientificScore(){
    int gear = 0, tablet = 0, compass = 0;
    for (DMAG::Card const& card : this->cards_played) {
        if (card.GetType() != CARD_TYPE::scientific)
            continue;

        switch (card.GetId()) {
            case CARD_ID::workshop:
            case CARD_ID::laboratory:
            case CARD_ID::observatory:
            case CARD_ID::study:
                gear++; break;

            case CARD_ID::apothecary:
            case CARD_ID::dispensary:
            case CARD_ID::lodge:
            case CARD_ID::academy:
                compass++; break;

            case CARD_ID::scriptorium:
            case CARD_ID::library:
            case CARD_ID::school:
            case CARD_ID::university:
                tablet++; break;

            case CARD_ID::scientists:
                this->sci_extra++;
        }
    }

    // TODO: choose the most advantageous scientific piece (this->sci_extra)

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
                             guild_score + science_score + this->victory_tokens - this->defeat_tokens;

    if ( this->board.GetStage() > 0) {
        int wonder_score = this->board.GetWonderPoints();
        this->victory_points += wonder_score;
    }

    return this->victory_points;
}


////////////////////////////
// WONDER-RELATED EFFECTS //
////////////////////////////

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
void Player::BuildDiscardFree(DMAG::Card c, std::vector<DMAG::Card> discard_pile){
    int id = this->board.GetType();
    int stage = this->board.GetStage();

    if ((id == WONDER_ID::halikarnassos_a && stage == 2) ||
        (id == WONDER_ID::halikarnassos_b && stage >= 1)) {
            this->BuildStructure(c, discard_pile, true);
    }
}

// Called ONCE per age.
void Player::BuildHandFree(DMAG::Card c){
    if (this->board.GetType() == WONDER_ID::olympia_a && this->board.GetStage() >= 2) {
        this->BuildStructure(c, this->cards_hand, true);
    }
}

// Called at the END of the game, before scoring.
// Where:
//       Card c      ->  Guild to be copied
//       int side 0  ->  GetEastNeighbor
//       int side 1  ->  GetWestNeighbor
void Player::CopyGuild(DMAG::Card c, int side){
    Player* neighbor = side ? this->GetWestNeighbor() : this->GetEastNeighbor();
    int id = this->board.GetType();
    int stage = this->board.GetStage();

    if (c.GetType() == CARD_TYPE::guild && (id == WONDER_ID::olympia_b && stage >= 3)) {
        for (DMAG::Card const& card : neighbor->cards_played) {
            if (c.Equal(card)) {
                this->cards_played.push_back(c);
                return;
            }
        }
    }
}

///////////////////////
// GETTERS & SETTERS //
///////////////////////

int Player::GetShields() {
    return this->resources[RESOURCE::shields];
}

DMAG::Wonder Player::GetBoard() {
    return this->board;
}

int Player::GetDefeatTokens() {
    return this->defeat_tokens;
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
    // Wonders provide specific initial resources
    this->resources[this->board.GetProduction()] += 1;
}

void Player::SetId(int id){
    this->id = id;
}

int Player::GetId(){
    return this->id;
}

}
