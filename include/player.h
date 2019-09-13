#ifndef PLAYER_H
#define PLAYER_H

#include <deck.h>
#include <card.h>

namespace DMAG {


class Player
{
private:
    int score,
        victory_points;
        Deck _deck;
public:
    Player();
    void CalculateScore();
    int GetScore();

};
}
#endif // PLAYER_H
