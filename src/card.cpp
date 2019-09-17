#include "card.h"
#include <string>

namespace DMAG {

Card::Card()
{
this->_type = CARD_TYPE::materials;
}

Card::Card(int _id, std::string _name, int _value, int _type){
    this->_id = _id;
    this->_name = _name;
    this->_type = _type;
    this->_value = _value;
}

std::string Card::GetName(){
    return this->_name;
}

int Card::GetValue() {
    return this->_value;
}
}
