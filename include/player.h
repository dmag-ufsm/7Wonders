#ifndef PLAYER_H
#define PLAYER_H

#include <deck.h>
#include <card.h>
#include <list>

namespace DMAG {


class Player
{
private:
    int score,
        victory_points;
        Deck _deck;
        void CalculateScore();
public:
    Player();
    int GetScore();
    void ReceiveCard(Card);
    void ReceiveCards(std::list<Card>);

};
}
#endif // PLAYER_H
