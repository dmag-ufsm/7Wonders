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
    unsigned int id;
    std::vector<DMAG::Card> cards_hand;
    std::vector<DMAG::Card> cards_played;
    DMAG::Wonder board;
    unsigned char coins;
    // resources (?)
    unsigned char shields;
    signed char conflict_tokens;
    unsigned char victory_points;

    DMAG::Player* player_east;
    DMAG::Player* player_west;

public:

    Player();
    void BuildWonder();
    void BuildStructure(DMAG::Card c);
    void BuildGuild();
    std::vector<Card> GetHandCards();
    void ReceiveCards(std::vector<Card> _cards_hand);
    DMAG::Card Discard();
    int BuyResource();
    void Battle(int age);

    int CalculateCivilianScore();
    int CalculateCommercialScore();
    int CalculateGuildScore();
    int CalculateWonderScore();
    int CalculateScientificScore(int gear, int tablet, int compass);
    int CalculateScore();

    int GetShields();
    DMAG::Player* GetEastNeighbor();
    DMAG::Player* GetWestNeighbor();

    void SetNeighbors(DMAG::Player* east, DMAG::Player* west);
    void SetWonder(DMAG::Wonder _board);
    void SetId(int id);

    int GetId();
};

}
#endif // PLAYER_H
