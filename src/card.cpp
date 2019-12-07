#include <card.h>
#include <string>
#include <utility>

namespace DMAG {

Card::Card(){}

Card::Card(int id, std::string name, int type, int era, int freeWithId, std::vector<int> cost, std::vector<int> amountPerPlayers){
    this->id = id;
    this->name = std::move(name);
    this->type = type;
    this->era = era;
    this->freeWithId = freeWithId;

    // must have a better way to deal with the cost and quantity of units by number of players
    // (but this one is working)

    for (int i = 0; i < 5; i++)
        this->amountPerPlayers[i] = amountPerPlayers[i];
    
    this->cost[RESOURCE::wood] = cost[0];
    this->cost[RESOURCE::ore] = cost[1];
    this->cost[RESOURCE::clay] = cost[2];
    this->cost[RESOURCE::stone] = cost[3];
    this->cost[RESOURCE::loom] = cost[4];
    this->cost[RESOURCE::glass] = cost[5];
    this->cost[RESOURCE::papyrus] = cost[6];
    this->cost[RESOURCE::coins] = cost[7];

}

int Card::GetId() const{
    return this->id;
}

std::string Card::GetName() const{
    return this->name;
}

int Card::GetType() const{
    return this->type;
}

int Card::GetEra() const{
    return this->era;
}

int Card::GetFreeWith() const{
    return this->freeWithId;
}

int Card::GetAmount(int players) const{
    return this->amountPerPlayers[players-3];
}

bool Card::Equal(Card c){
    return this->GetId() == c.GetId();
}

bool Card::CanBePlayed(std::map<int, unsigned char> resources){
    return (this->cost[RESOURCE::wood] <= resources[RESOURCE::wood] &&
            this->cost[RESOURCE::ore] <= resources[RESOURCE::ore] &&
            this->cost[RESOURCE::clay] <= resources[RESOURCE::clay] &&
            this->cost[RESOURCE::stone] <= resources[RESOURCE::stone] &&
            this->cost[RESOURCE::loom] <= resources[RESOURCE::loom] &&
            this->cost[RESOURCE::glass] <= resources[RESOURCE::glass] &&
            this->cost[RESOURCE::papyrus] <= resources[RESOURCE::papyrus] &&
            this->cost[RESOURCE::coins] <= resources[RESOURCE::coins]);
}

std::map<int, unsigned char> Card::MissingCards(std::map<int, unsigned char> resources) {
    std::map<int, unsigned char> missing{
        { RESOURCE::wood, 0 },
        { RESOURCE::ore, 0 },
        { RESOURCE::clay, 0 },
        { RESOURCE::stone, 0 },
        { RESOURCE::loom, 0 },
        { RESOURCE::glass, 0 },
        { RESOURCE::papyrus, 0 },
        { RESOURCE::coins, 0 }
    };

    for (std::map<int, unsigned char>::iterator it = missing.begin(); it!=missing.end(); ++it) {
        if (resources[it->first] < this->cost[it->first]) {
            missing[it->first] = this->cost[it->first] - resources[it->first];
        }
    }

    return missing;
}

}
