#ifndef DECK_H
#define DECK_H

#include <card.h>
#include <stack>
namespace DMAG {

class Deck
{
private:
    std::stack<Card> _deck;
    std::stack<Card> _discard;
public:
    Deck();
    void Shuffle();
    void IncludeGuild();
    Card GetTop();
    bool IsEmpty();
};

}
#endif // DECK_H
