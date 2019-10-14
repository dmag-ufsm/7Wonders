#ifndef PLAYER_H
#define PLAYER_H

#include <card.h>
#include <list>

namespace DMAG {


class Player
{
private:
    int score,
        victory_points;
	std::list<Card> hand;

public:
    Player();
    void CalculateScore();
    int GetScore();
	void GiveCards();

};
}
#endif // PLAYER_H
