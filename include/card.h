#ifndef CARD_H
#define CARD_H

#include <string>

namespace CARD_TYPE {
enum type{
    guild,
    research,
    army,
    resource,

};

}

namespace DMAG {

class Card
{
private:
    int _id;
    std::string _name;
    int _value;
    int _type;

public:
    Card();
    Card(int, std::string, int, int);
    std::string GetName();
    int GetValue();
};

}
#endif // CARD_H
