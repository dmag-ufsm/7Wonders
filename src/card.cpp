#include "card.h"
#include <string>
#include <utility>

namespace DMAG {

Card::Card(int id, std::string name) {
    this->id = id;
    this->name = std::move(name);
}

std::string Card::GetName() const{
    return this->name;
}

int Card::GetId() const{
    return this->id;
}

}
