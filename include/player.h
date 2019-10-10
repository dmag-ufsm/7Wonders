#ifndef PLAYER_H
#define PLAYER_H

#include <card.h>
#include <vector>

namespace DMAG {


class Player
{
private:
    std::vector<Card> cards;
    int score;
    int victory_points;
    int conflict_tokens;
    int coins;
    bool wonder_built;
    // board?
    // wonder?

public:
    Player();
    void CalculateScore();
    int GetScore();
    void Discard(Card c);
    void Battle(Player p);

};
}
#endif // PLAYER_H
