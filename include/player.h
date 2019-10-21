#ifndef PLAYER_H
#define PLAYER_H

#include <card.h>
#include <wonder.h>
#include <list>
#include <vector>

namespace DMAG {

class Player
{
private:
    std::vector<DMAG::Card> cards_hand;
    std::vector<DMAG::Card> cards_played;
    DMAG::Wonder board;
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
    std::vector<Card> GetHandCards();
    void ReceiveCards(std::vector<Card> _cards_hand);
    DMAG::Card Discard();
    int BuyResource();
    void Battle(int age);

    int CalculateCivilianScore();
    int CalculateCommercialScore();
    int CalculateGuildScore();
    int CalculateScientificScore(int gear, int tablet, int compass);
    int CalculateScore();

    int GetShields();
    DMAG::Player* GetEastNeighbor();
    DMAG::Player* GetWestNeighbor();

    void SetNeighbours(DMAG::Player *east, DMAG::Player *west);
    void SetWonder(DMAG::Wonder _board);
};

}
#endif // PLAYER_H
