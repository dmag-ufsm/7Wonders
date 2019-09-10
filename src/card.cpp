#include "card.h"

namespace DMAG {

Card::Card()
{
this->_type = CARD_TYPE::research;
}

Card::Card(int _id, std::string _name, int _value, int _type){
    this->_id = _id;
    this->_name = _name;
    this->_type = _type;
    this->_value = _value;
}
}
