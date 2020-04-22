#include <player.h>
#include <algorithm>
#include <iostream>

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
    this->free_card_once = true; // Olympia 2A -> can build one card for free once per Age.
    this->discard_free = false;

    this->used_tree_farm = -1;
    this->used_forest_cave = -1;
    this->used_timber_yard = -1;
    this->used_excavation = -1;
    this->used_mine = -1;
    this->used_clay_pit = -1;
    this->used_forum = -1;
    this->used_caravansery = -1;

    this->raw_extra = false;
    this->manuf_extra = false;

    this->player_east = NULL;
    this->player_west = NULL;
}

//////////////////
// CARD-RELATED //
//////////////////

bool Player::BuildWonder(DMAG::Card c){
    // 1) Build Wonder stage if possible.
    // 2) If the stage was built, remove card 'c' from the hand.
    if (this->board->AddStage(this)) {
        size_t i = 0;
        for (i = 0; i < this->cards_hand.size(); i++) {
            if (this->cards_hand[i].Equal(c)) break;
        }
        this->cards_hand.erase(this->cards_hand.begin()+i);
        std::cout << this->id << " -> SUCCESS -> " << "builds a " << this->board->GetName() << " stage!" << std::endl;
        return true;
    }

    std::cout << this->id << " -> FAILURE -> " << "couldn't build the next " << this->board->GetName() << " stage." << std::endl;
    return false;
}

// Play a card.
// * arg cards is necessary for any special situation? it is equal to this->cards_hand
// P: Yes, in case we want to build a free card from the discard pile.
bool Player::BuildStructure(DMAG::Card c, std::vector<DMAG::Card> cards, bool free_card){
    // Returns false if the card has already been played (cannot play the same card twice).
    for (DMAG::Card const& card : this->cards_played)
        if (c.Equal(card)) return false;

    // Checks if card is in hand.
    size_t i = 0;
    for (i = 0; i < cards.size(); i++) {
        //std::cout << cards[i].GetName() << std::endl;
        if (cards[i].Equal(c)) break;
    }
    if (i == cards.size()) {
        // ERROR: Card played was not found in the vector
        return false;
    }

    if (!free_card) {
        // If there are not enough resources.
        if (!c.CanBePlayed(this->resources)) {
            // Check if the card can be played for free, otherwise try to produce the resources needed.
            if (this->CheckFreeCard(c)) free_card = true;
            else {
                Player* east = this->GetEastNeighbor();
                Player* west = this->GetWestNeighbor();
                std::map<int, int> resources_east = east->GetResources();
                std::map<int, int> resources_west = west->GetResources();
                std::map<int, int> resources_bckp = this->resources;
                std::map<int, int> resources_needed = c.MissingCards(this->resources);
                for (std::map<int, int>::iterator it = resources_needed.begin();
                     it!=resources_needed.end(); ++it) {
                    int quant_needed = resources_needed[it->first];
                    if (quant_needed > 0) {
                        bool could_produce = this->ProduceResource(it->first, quant_needed);
                        // If it was not possible to produce/buy, revert changes and return.
                        if (!could_produce) {
                            this->resources = resources_bckp;
                            this->ResetUsed(false);
                            east->SetResources(resources_east);
                            east->ResetUsed(false);
                            west->SetResources(resources_west);
                            west->ResetUsed(false);
                            std::cout << this->id << " -> FAILURE -> " << "couldn't produce resources." << std::endl;
                            return false;
                        }
                    }
                }
            }
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
                // wood or clay (won't be applied here, see ProduceResource)
                cost = 1;
            } else if (card == CARD_ID::excavation) {
                // stone or clay (won't be applied here, see ProduceResource)
                cost = 1;
            } else if (card == CARD_ID::clay_pit) {
                // clay or ore (won't be applied here, see ProduceResource)
                cost = 1;
            } else if (card == CARD_ID::timber_yard) {
                // stone or wood (won't be applied here, see ProduceResource)
                cost = 1;
            } else if (card == CARD_ID::forest_cave) {
                // wood or ore (won't be applied here, see ProduceResource)
                cost = 1;
            } else if (card == CARD_ID::mine) {
                // ore or stone (won't be applied here, see ProduceResource)
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
                this->manuf_extra = true;
            } else if (card == CARD_ID::caravansery) {
                this->raw_extra = true;
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
                int stages = this->board->GetStage();
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

        default:
            break;
    }

    cards_hand.erase(cards_hand.begin()+i);
    cards_played.push_back(c);

    if (!free_card) this->resources[RESOURCE::coins] -= cost;
    std::cout << this->id << " -> SUCCESS -> " << "builds " << c.GetName() << std::endl;

    return true;
}

// Checks if the player has a card that permits him to play card 'c' for free.
bool Player::CheckFreeCard(DMAG::Card c){
    int free_id = c.GetFreeWith();

    if (free_id != CARD_ID::none) {
        // Treating exception:
        // Forum can be constructed for free if the player has either the East Trading Post
        // or the West Trading Post (raw_cheap_east or raw_cheap_west).
        if (free_id == -1 && (this->raw_cheap_east || this->raw_cheap_west))
            return true;

        // General case:
        for (DMAG::Card const& card : this->cards_played) {
            if (card.GetId() == free_id) return true;
        }
    }

    return false;
}

std::vector<Card> Player::GetHandCards(){
    return this->cards_hand;
}

std::vector<Card> Player::GetPlayedCards(){
    return this->cards_played;
}

std::vector<Card> Player::GetPlayableCards(){
    this->cards_playable.clear();

    std::map<int, int> this_bckp = this->resources;

    Player* east = this->GetEastNeighbor();
    std::map<int, int> east_bckp = east->GetResources();

    Player* west = this->GetWestNeighbor();
    std::map<int, int> west_bckp = west->GetResources();

    for (DMAG::Card c : this->cards_hand) {
        bool push_card = true;
        //std::cout << " Checking ---> " << c.GetName() << std::endl;
        // Check if you can play the card directly.
        if (c.CanBePlayed(this->resources) || this->CheckFreeCard(c)) {
            //std::cout << "         Can Play ---> " << c.GetName() << std::endl;
            this->cards_playable.push_back(c);
        }
        // Check if you can potentially play the card.
        else {
            std::map<int, int> resources_needed = c.MissingCards(this->resources);
            for (std::map<int, int>::iterator it = resources_needed.begin();
                 it != resources_needed.end(); ++it) {
                int quant_needed = resources_needed[it->first];
                if (quant_needed > 0) {
                    bool could_produce = this->ProduceResource(it->first, quant_needed);
                    if (!could_produce) { push_card = false; break; }
                }
            }
            if (push_card) this->cards_playable.push_back(c);
        }
        // Undo all changes on the resource maps.
        this->ResetUsed(false);
        this->resources = this_bckp;
        east->ResetUsed(false);
        east->SetResources(east_bckp);
        west->ResetUsed(false);
        west->SetResources(west_bckp);
    }

    return this->cards_playable;
}

void Player::ReceiveCards(std::vector<Card> _cards_hand){
    this->cards_hand = _cards_hand;
}

void Player::Discard(DMAG::Card c){
	// This just adds some coins to the player total.
    this->resources[RESOURCE::coins] += 3;

    int i;
    for (i = 0; i < this->cards_hand.size(); i++) {
        if (this->cards_hand[i].Equal(c)) break;
    }
    this->cards_hand.erase(this->cards_hand.begin()+i);
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

bool Player::AvailableCard(int card_id, int resource){
   for (DMAG::Card const& card : this->cards_played) {
       int this_card = card.GetId();
       if (this_card == card_id) {
           if (this_card == CARD_ID::tree_farm && this->used_tree_farm == -1) {
               this->used_tree_farm = resource;
               return true;
           }
           if (this_card == CARD_ID::forest_cave && this->used_forest_cave == -1) {
               this->used_forest_cave = resource;
               return true;
           }
           if (this_card == CARD_ID::timber_yard && this->used_timber_yard == -1) {
               this->used_timber_yard = resource;
               return true;
           }
           if (this_card == CARD_ID::excavation && this->used_excavation == -1) {
               this->used_excavation = resource;
               return true;
           }
           if (this_card == CARD_ID::mine && this->used_mine == -1) {
               this->used_mine = resource;
               return true;
           }
           if (this_card == CARD_ID::clay_pit && this->used_clay_pit == -1) {
               this->used_clay_pit = resource;
               return true;
           }
           if (this_card == CARD_ID::forum && this->used_forum == -1) {
               this->used_forum = resource;
               return true;
           }
           if (this_card == CARD_ID::caravansery && this->used_caravansery == -1) {
               this->used_caravansery = resource;
               return true;
           }
       }
   }
   return false;
}

bool Player::HasPlayedCard(DMAG::Card c){
   for (DMAG::Card const& card : this->cards_played) {
       if (c.Equal(card)) return true;
    }
   return false;
}

// Prepare for the next turn.
// - called at the end of a turn.
void Player::ResetUsed(bool decrement) {
    if (decrement) this->DecrementUsed();
    this->used_tree_farm    = -1;
    this->used_forest_cave  = -1;
    this->used_timber_yard  = -1;
    this->used_excavation   = -1;
    this->used_mine         = -1;
    this->used_clay_pit     = -1;
    this->used_forum        = -1;
    this->used_caravansery  = -1;
}


//////////////////////
// RESOURCE-RELATED //
//////////////////////

// Check if the player can produce a resource (in cases where a certain structure gives
// one resource OR the other; that is, "on demand" resources).
// If he can't produce by himself, try buying it from neighbors.
// - this function is a "step" in BuildStructure.
bool Player::ProduceResource(int resource, int quant){
    //std::cout <<  "  -> " << "has " << (int)this->resources[resource] << " needs " << quant << std::endl;
    // Extra check to verify if the player can build directly without needing to produce.
    if (this->resources[resource] >= quant) {
        std::cout <<  "  -> " << "already has the needed resource!" << std::endl;
        return true;
    }

    // Produce "on demand" resources.
    // Ugly, but hopefully it'll get the job done.
    int needed = this->IncrementOnDemand(resource, quant, false);
    if (needed <= 0) {
        std::cout <<  "  -> " << "produced the resource successfully!" << std::endl;
        return true;
    }

    // if the player couldn't produce the resource by himself, try buying it from a neighbor.
    return this->BuyResource(resource, needed);
}

// Some cards can produce one resource OR the other each turn. This function takes care of this by
// incrementing the needed resource if a structure that fits the criteria is available (not used already).
// -> If "is_neighbor" = true, don't increment even if they can ChooseExtraManuf or ChooseExtraRaw, as the "extra"
//    resource is not tradeable.
// Returns the needed resources - quantity produced.
int Player::IncrementOnDemand(int resource, int needed, bool is_neighbor){
    // From: https://github.com/dmag-ufsm/Game/blob/master/references/cards.csv
    // Wood                 -> tree_farm  || forest_cave || timber_yard || caravansery
    // Clay                 -> tree_farm  || excavation  || clay_pit    || caravansery
    // Stone                -> excavation || timber_yard || mine        || caravansery
    // Ore                  -> clay_pit   || forest_cave || mine        || caravansery
    // Glass, Loom, Papyrus -> forum

    int cont = 0;
    if (resource == RESOURCE::wood) {
        if (this->AvailableCard(CARD_ID::tree_farm, resource)) {
            cont++;
        } if (this->AvailableCard(CARD_ID::forest_cave, resource)) {
            cont++;
        } if (this->AvailableCard(CARD_ID::timber_yard, resource)) {
            cont++;
        } if (this->AvailableCard(CARD_ID::caravansery, resource)) {
            cont++;
        }
    } else if (resource == RESOURCE::clay) {
        if (this->AvailableCard(CARD_ID::tree_farm, resource)) {
            cont++;
        } if (this->AvailableCard(CARD_ID::excavation, resource)) {
            cont++;
        } if (this->AvailableCard(CARD_ID::clay_pit, resource)) {
            cont++;
        } if (this->AvailableCard(CARD_ID::caravansery, resource)) {
            cont++;
        }
    } else if (resource == RESOURCE::stone) {
        if (this->AvailableCard(CARD_ID::excavation, resource)) {
            cont++;
        } if (this->AvailableCard(CARD_ID::timber_yard, resource)) {
            cont++;
        } if (this->AvailableCard(CARD_ID::mine, resource)) {
            cont++;
        } if (this->AvailableCard(CARD_ID::caravansery, resource)) {
            cont++;
        }
    } else if (resource == RESOURCE::ore) {
        if (this->AvailableCard(CARD_ID::clay_pit, resource)) {
            cont++;
        } if (this->AvailableCard(CARD_ID::forest_cave, resource)) {
            cont++;
        } if (this->AvailableCard(CARD_ID::mine, resource)) {
            cont++;
        } if (!is_neighbor && this->AvailableCard(CARD_ID::caravansery, resource)) {
            cont++;
        }
    } else if (resource == RESOURCE::glass || resource == RESOURCE::loom || resource == RESOURCE::papyrus) {
        if (!is_neighbor && this->AvailableCard(CARD_ID::forum, resource)) {
            cont++;
        } if (!is_neighbor && this->ChooseExtraManuf(resource)) {
            cont++;
        }
    }

    if (resource <= 3 && !is_neighbor) { // resource <= 3 is a raw
        if (this->ChooseExtraRaw(resource)) cont++;
        if (this->raw_extra) cont++;
    }

    this->resources[resource] += cont;
    return (needed - this->resources[resource]); // If this ends up <= 0, the resource was produced.
}

// The resources we incremented "on demand" for a turn should be decremented.
// Part of ResetUsed to be called at the end of a turn.
void Player::DecrementUsed(){
    if (this->used_tree_farm >= 0)    this->resources[this->used_tree_farm]--;
    if (this->used_forest_cave >= 0)  this->resources[this->used_forest_cave]--;
    if (this->used_timber_yard >= 0)  this->resources[this->used_timber_yard]--;
    if (this->used_excavation >= 0)   this->resources[this->used_excavation]--;
    if (this->used_mine >= 0)         this->resources[this->used_mine]--;
    if (this->used_clay_pit >= 0)     this->resources[this->used_clay_pit]--;
    if (this->used_forum >= 0)        this->resources[this->used_forum]--;
    if (this->used_caravansery >= 0)  this->resources[this->used_caravansery]--;
}

// Buys x quantity of a certain resource from any neighbor.
// - this function is a "step" in BuildStructure.
bool Player::BuyResource(int resource, int quant){
    bool is_raw = resource <= 3 ? true : false; // raw materials have code <= 3 in resources.h
    int cost = 2*quant;
    cost = !is_raw && this->manuf_cheap ? 1*quant : 2*quant;

    Player* east = this->GetEastNeighbor();
    Player* west = this->GetWestNeighbor();

    // Wonder initial resource is not buyable!
    east->AddResource(east->board->GetProduction(), -1);
    west->AddResource(west->board->GetProduction(), -1);

    int needed_east = east->IncrementOnDemand(resource, quant, true);
    int needed_west = west->IncrementOnDemand(resource, quant, true);

    if (needed_east <= 0) { // neeed_east/west <= 0 means that the neightbor has the resource.
        if (is_raw && (this->raw_cheap_east || this->wonder_raw_cheap)) cost = 1*quant;
        if (this->resources[RESOURCE::coins] >= cost) {
            this->resources[RESOURCE::coins] -= cost;
            east->AddResource(RESOURCE::coins, cost);
            east->AddResource(east->board->GetProduction(), 1);
            west->AddResource(west->board->GetProduction(), 1);
            //east->DecrementUsed();
            //std::cout <<  "  -> " << "bought the resource successfully!" << std::endl;
            return true;
        }
    }
    if (needed_west <= 0) {
        if (is_raw && (this->raw_cheap_west || this->wonder_raw_cheap)) cost = 1*quant;
        if (this->resources[RESOURCE::coins] >= cost) {
            this->resources[RESOURCE::coins] -= cost;
            west->AddResource(RESOURCE::coins, cost);
            west->AddResource(west->board->GetProduction(), 1);
            east->AddResource(east->board->GetProduction(), 1);
            //west->DecrementUsed();
            //std::cout <<  "  -> SUCCESS -> " << "bought the resource successfully!" << std::endl;
            return true;
        }
    }

    east->AddResource(east->board->GetProduction(), 1);
    west->AddResource(west->board->GetProduction(), 1);

    //std::cout <<  "  -> FAILURE -> " << "couldn't buy the resource." << std::endl;
    return false; // Couldn't buy
}

// Adds x quantity of a certain resource.
void Player::AddResource(int resource, int quant){
    this->resources[resource] += quant;
}

// Returns true if the player has x quantity of a certain resource.
bool Player::HasEnoughResource(int resource, int quant){
    return (this->resources[resource] >= quant);
}


////////////////////
// BATTLE-RELATED //
////////////////////

// The player battles each neighbor at the end of every Age.
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

    // Battle with east neighbor
    if (this_shields > player_east->GetShields()) {
        this->victory_tokens += current_age_value;
        std::cout <<  this->id << " -> WON battle with " << player_east->GetId() << std::endl;
    } else if (this_shields < player_east->GetShields()) {
        std::cout <<  this->id << " -> LOST battle with " << player_east->GetId() << std::endl;
        this->defeat_tokens += 1;
    } else {
        std::cout <<  this->id << " -> DRAW battle with " << player_east->GetId() << std::endl;
    }

    // Battle with west neighbor
    if (this_shields > player_west->GetShields()) {
        std::cout <<  this->id << " -> WON battle with " << player_west->GetId() << std::endl;
        this->victory_tokens += current_age_value;
    } else if (this_shields < player_west->GetShields()) {
        std::cout <<  this->id << " -> LOST battle with " << player_west->GetId() << std::endl;
        this->defeat_tokens += 1;
    } else {
        std::cout <<  this->id << " -> DRAW battle with " << player_west->GetId() << std::endl;
    }
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
                comm_score += this->board->GetStage();
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
                guild_score = player_east->GetBoard()->GetStage()
                            + player_west->GetBoard()->GetStage()
                            + this->board->GetStage();
                break;
        }
    }

    return guild_score;
}

// Calculates the number of VPs the player gets from scientific development.
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

    int id = this->board->GetId();
    int stage = this->board->GetStage();
    if ((id == WONDER_ID::babylon_a && stage >= 2) ||
            (id == WONDER_ID::babylon_b && stage >= 3)) {
        this->sci_extra++;
    }

    // Choose the most advantageous scientific piece for extra piece:
    // First complete the set if possible. The pieces left over, add to what has more.
    int remaining_tablets = 0, remaining_compasses = 0, remaining_gears = 0, extra = this->sci_extra;
    if (tablet >= compass && tablet >= gear) {
        remaining_compasses = tablet - compass;
        remaining_gears = tablet - gear;
        if (remaining_compasses + remaining_gears <= this->sci_extra) {
            compass += remaining_compasses;
            gear += remaining_gears;
            extra -= remaining_compasses + remaining_gears;
        }
        tablet += extra;
    } else if (compass >= tablet && compass >= gear) {
        remaining_tablets = compass - tablet;
        remaining_gears = compass - gear;
        if (remaining_tablets + remaining_gears <= extra) {
            tablet += remaining_tablets;
            gear += remaining_gears;
            extra -= remaining_tablets + remaining_gears;
        }
        compass += extra;
    } else if (gear >= tablet && gear >= compass) {
        remaining_tablets = gear - tablet;
        remaining_compasses = gear - compass;
        if (remaining_compasses + remaining_tablets <= extra) {
            compass += remaining_compasses;
            tablet += remaining_tablets;
            extra -= remaining_compasses + remaining_tablets;
        }
        gear += extra;
    }

    // The smallest value among the three is the amount of completed sets
    int completed_sets = 0;
    if (gear <= tablet && gear <= compass)    completed_sets = gear;
    if (tablet <= gear && tablet <= compass)  completed_sets = tablet;
    if (compass <= gear && compass <= tablet) completed_sets = compass;

    int points_per_set_completed = 7;
    return (gear * gear) + (tablet * tablet) + (compass * compass) + (completed_sets * points_per_set_completed);
}

// Calculates the player's TOTAL score using the methods defined above.
int Player::CalculateScore(){
    int treasury_score = static_cast<int>(this->resources[RESOURCE::coins]/3); // 1 VP for every 3 coins
    int civil_score = this->CalculateCivilianScore();
    int commercial_score = this->CalculateCommercialScore();
    int guild_score = this->CalculateGuildScore();
    int science_score = this->CalculateScientificScore();

    int total_score = treasury_score + civil_score + commercial_score + guild_score +
                       science_score + this->victory_tokens - this->defeat_tokens;

    if (this->board->GetStage() > 0)
        total_score += this->board->GetWonderPoints();

    this->victory_points = total_score;

    return this->victory_points;
}


////////////////////////////
// WONDER-RELATED EFFECTS //
////////////////////////////

// The player chooses one of the 3 manufactured resources to get for free at the end of the game.
// - called ONCE each turn; part of IncrementOnDemand().
bool Player::ChooseExtraManuf(int resource){
    if (resource == RESOURCE::loom || resource == RESOURCE::glass || resource == RESOURCE::papyrus) {
        if (this->board->GetId() == WONDER_ID::alexandria_b && this->board->GetStage() >= 2) {
            //this->resources[resource]++;
            return true;
        }
    }
    return false;
}


// The player chooses one of the 4 raw resources to receive for free at each turn (untradable).
// - called ONCE each turn; part of IncrementOnDemand().
bool Player::ChooseExtraRaw(int resource){
    if (resource == RESOURCE::wood || resource == RESOURCE::ore ||
        resource == RESOURCE::clay || resource == RESOURCE::stone) {
        int id = this->board->GetId();
        int stage = this->board->GetStage();
        if ((id == WONDER_ID::alexandria_a && stage >= 2) ||
            (id == WONDER_ID::alexandria_b && stage >= 1)) {
            //this->resources[resource]++;
            return true;
        }
    }
    return false;
}

// The player can choose to play the seventh card of an Age instead of discarding it.
// - automatically called when Babylon B stage 2 is constructed.
void Player::CanPlaySeventh(){
    this->play_seventh = true;
}

bool Player::PlaySeventh(){
    return this->play_seventh;
}

// Called at the end of each age to permit the player to build another card
// for free if he has the ability to do so. If he doesn't have such ability,
// free_card_once is just another useless bool :(
void Player::FreeCardOnce(bool flag) {
    this->free_card_once = flag;
}

// Similar as above but for building a free structure from the discard list.
void Player::DiscardFree(bool flag) {
    this->discard_free = flag;
}

// The player can buy resources for 1 coin instead of 2 from neighbors.
// - automatically called when Olympia A stage 1 is constructed.
void Player::CanBuyRawCheap(){
    this->wonder_raw_cheap = true;
}

// Builds a card from the discard pile for free.
// - will be changed depending on which type discard_pile will be.
// - called at the end of the turn after the stage was built.
bool Player::BuildDiscardFree(DMAG::Card c, std::vector<DMAG::Card> discard_pile){
    int id = this->board->GetId();
    int stage = this->board->GetStage();

    if ((id == WONDER_ID::halikarnassos_a && stage == 2) ||
        (id == WONDER_ID::halikarnassos_b && stage >= 1)) {
        if (this->discard_free) {
            this->discard_free = false;
            return this->BuildStructure(c, discard_pile, true);
        }
    }
    return false;
}

// Builds a card from the hand for free, once per Age, if the player has the requirements.
bool Player::BuildHandFree(DMAG::Card c){
    if (this->board->GetId() == WONDER_ID::olympia_a && this->board->GetStage() >= 2) {
        if (this->free_card_once) {
            this->free_card_once = false; // now the player won't be able to build for free in the same Age
            return this->BuildStructure(c, this->cards_hand, true);
        }
    }
    return false;
}

// Copies a neighbor's Guild based on which one provides more VPs.
// - called at the END of the game, before scoring.
// UNTESTED!
bool Player::CopyGuild(){
    Player* east = this->GetEastNeighbor();
    Player* west = this->GetWestNeighbor();

    int id = this->board->GetId();
    int stage = this->board->GetStage();

    // first -> guild; second -> total VPs it can give to the current player.
    std::map<int, int> guild_vps{
            { CARD_ID::workers, 0 },
            { CARD_ID::craftsmens, 0 },
            { CARD_ID::traders, 0 },
            { CARD_ID::philosophers, 0 },
            { CARD_ID::spies, 0 },
            { CARD_ID::magistrates, 0 },
            { CARD_ID::shipowners, 0 },
            { CARD_ID::strategists, 0 },
            { CARD_ID::scientists, 0 },
            { CARD_ID::builders, 0 },
    };

    if (id == WONDER_ID::olympia_b && stage >= 3) {
        std::vector<DMAG::Card> available_guilds;
        for (DMAG::Card const& card : east->cards_played) {
            if (card.GetType() == CARD_TYPE::guild) available_guilds.push_back(card);
        }
        for (DMAG::Card const& card : west->cards_played) {
            if (card.GetType() == CARD_TYPE::guild) available_guilds.push_back(card);
        }
        // For each guild the neighbors have, calculate the potential VPs it can give
        // to the current player.
        // The guild with the most VPs will be the chosen one to be copied.
        for (DMAG::Card const& guild : available_guilds) {
            int guild_id = guild.GetId();
            switch (guild_id) {
                case CARD_ID::workers:
                    guild_vps[CARD_ID::workers] = east->AmountOfType(CARD_TYPE::materials) + west->AmountOfType(CARD_TYPE::materials);
                    break;
                case CARD_ID::craftsmens:
                    guild_vps[CARD_ID::craftsmens] = east->AmountOfType(CARD_TYPE::manufactured) + west->AmountOfType(CARD_TYPE::manufactured);
                    break;
                case CARD_ID::traders:
                    guild_vps[CARD_ID::traders] = east->AmountOfType(CARD_TYPE::commercial) + west->AmountOfType(CARD_TYPE::commercial);
                    break;
                case CARD_ID::philosophers:
                    guild_vps[CARD_ID::philosophers] = east->AmountOfType(CARD_TYPE::scientific) + west->AmountOfType(CARD_TYPE::scientific);
                    break;
                case CARD_ID::spies:
                    guild_vps[CARD_ID::spies] = east->AmountOfType(CARD_TYPE::military) + west->AmountOfType(CARD_TYPE::military);
                    break;
                case CARD_ID::magistrates:
                    guild_vps[CARD_ID::magistrates] = east->AmountOfType(CARD_TYPE::civilian) + west->AmountOfType(CARD_TYPE::civilian);
                    break;
                case CARD_ID::shipowners:
                    guild_vps[CARD_ID::shipowners] = this->AmountOfType(CARD_TYPE::materials) + this->AmountOfType(CARD_TYPE::manufactured);
                    guild_vps[CARD_ID::shipowners] += this->AmountOfType(CARD_TYPE::guild);
                    break;
                case CARD_ID::strategists:
                    guild_vps[CARD_ID::strategists] = east->GetDefeatTokens() + west->GetDefeatTokens();
                    break;
                case CARD_ID::scientists:
                    // Not 100% accurate.
                    guild_vps[CARD_ID::scientists] = this->AmountOfType(CARD_TYPE::scientific);
                    break;
                case CARD_ID::builders:
                    guild_vps[CARD_ID::builders] = east->GetBoard()->GetStage() + west->GetBoard()->GetStage();
                    guild_vps[CARD_ID::builders] += this->GetBoard()->GetStage();
                    break;
                default:
                    break;
            }
        }
        std::map<int,int>::iterator best_guild
            = std::max_element(guild_vps.begin(), guild_vps.end(),[] (const std::pair<int,int>& a, const std::pair<int,int>& b)->bool{ return a.second < b.second; } );

        if (best_guild->second > 0) {
            for (DMAG::Card const& guild : available_guilds) {
                // Pushes the chosen guild to the vector of played cards.
                if (guild.GetId() == best_guild->first) {
                    this->cards_played.push_back(guild);
                    std::cout << this->id << " -> SUCCESS -> " << "copies guild " << guild.GetName() << std::endl;
                    return true;
                }
            }
        }
    }

    std::cout << this->id << " -> FAILURE -> " << "couldn't copy guild!" << std::endl;
    return false;
}

///////////////////////
// GETTERS & SETTERS //
///////////////////////

int Player::GetShields() {
    return this->resources[RESOURCE::shields];
}

DMAG::Wonder* Player::GetBoard() {
    return this->board;
}

int Player::GetDefeatTokens() {
    return this->defeat_tokens;
}

std::map<int, int> Player::GetResources(){
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

void Player::SetWonder(DMAG::Wonder *_board){
    this->board = _board;
    // Wonders provide specific initial resources
    this->resources[this->board->GetProduction()] += 1;
}

void Player::SetId(int id){
    this->id = id;
}

int Player::GetId(){
    return this->id;
}

void Player::SetResources(std::map<int, int> r){
    this->resources = r;
}

}
