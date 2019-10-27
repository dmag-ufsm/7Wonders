#include <card.h>
#include <string>
#include <utility>

namespace DMAG {

Card::Card(){}

Card::Card(int id, std::string name, int type, int era, std::vector<int> amount){
    this->id = id;
    this->name = std::move(name);
    this->type = type;
    this->era = era;
    for (int i = 0; i < 5; i++)
    	this->amount[i] = amount[i];
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

int Card::GetAmount(int players) const{
	return this->amount[players-3];
}

bool Card::Equal(Card c){
    return this->GetId() == c.GetId();
}

}
