#ifndef PLAYER_H
#define PLAYER_H

#include <card.h>

namespace DMAG {


class Player
{
private:
    int score,
        victory_points;

public:
    Player();
    void CalculateScore();
    int GetScore();

};
}
#endif // PLAYER_H
