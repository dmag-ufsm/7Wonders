#include <wonder.h>

namespace DMAG {

Wonder::Wonder(){}

Wonder::Wonder(int _id, std::string _name, int _production, std::list<int> _effects)
{
    this->id = _id;
    this->name = _name;
    this->production = _production;
    this->effects = _effects;
    this->stage = 0;
}

std::string Wonder::GetName() const{
    return this->name;
}

int Wonder::GetProduction() const{
    return this->production;
}

int Wonder::GetStage() const{
    return this->stage;
}

std::list<int> Wonder::GetEffects() const{
    return this->effects;
}

void Wonder::AddStage(){
    this->stage++;
}

}
