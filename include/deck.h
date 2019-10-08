#ifndef DECK_H
#define DECK_H

#include <card.h>
#include <list>
namespace DMAG {

class Deck
{
private:
    std::list<Card> _deck;
    std::list<Card> _discard;
public:
    Deck();
    void Shuffle();
    void IncludeGuild();
    Card GetTop();
    bool IsEmpty();
};

}
#endif // DECK_H
