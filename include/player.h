#ifndef PLAYER_H
#define PLAYER_H

#include <card.h>
#include <list>

namespace DMAG {

class Player
{
private:
    std::list<DMAG::Card> cards_hand;
    std::list<DMAG::Card> cards_played;
    unsigned char board; // wonder
    unsigned char coins;
    // resources (?)
    unsigned char shields;
    signed char conflict_tokens;
    unsigned char wonder_stage; // 0 to 3
    unsigned char victory_points;

    // The neighbour players will need to be pointers, otherwise
    // the compiler won't know how to deal with them
    DMAG::Player* player_east;
    DMAG::Player* player_west;

public:
    Player();
    void BuildWonder();
    void BuildStructure();
    void BuildGuild();
    void ReceiveCards();
    DMAG::Card Discard();
    int BuyResource();
    int Battle(DMAG::Player p, int age);
    int CalculateScientificScore(int gear, int tablet, int compass);
    int CalculateScore();

	void SetNeighbours(DMAG::Player *east, DMAG::Player * west);

    // Will probably need SetWonder, SetNeighbours?
};

}
#endif // PLAYER_H
