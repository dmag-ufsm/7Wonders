#ifndef PLAYER_H
#define PLAYER_H

#include <card.h>
#include <wonder.h>
#include <resources.h>
#include <vector>
#include <map>

namespace DMAG {

class Player
{
private:
    unsigned int id;
    std::vector<DMAG::Card> cards_hand;
    std::vector<DMAG::Card> cards_played;
    DMAG::Wonder board;
    // unsigned char coins; -> moved to resources map
    unsigned char shields;
    signed char conflict_tokens;
    unsigned char victory_points;

    // Key needs to be int because the underlying type in enums is int
    std::map<int, unsigned char> resources{
        { RESOURCE::wood, 0 },
        { RESOURCE::ore, 0 },
        { RESOURCE::clay, 0 },
        { RESOURCE::stone, 0 },
        { RESOURCE::cloth, 0 },
        { RESOURCE::glass, 0 },
        { RESOURCE::papyrus, 0 },
        { RESOURCE::gear, 0 },
        { RESOURCE::compass, 0 },
        { RESOURCE::tablet, 0 },
        { RESOURCE::coins, 3 }
    };

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
    void AddResource(int resource, int quant);
    void AddShield(int quant);
    void Battle(int age);

    int CalculateCivilianScore();
    int CalculateCommercialScore();
    int CalculateGuildScore();
    int CalculateScientificScore(int gear, int tablet, int compass);
    int CalculateScore();

    int GetShields();
    std::map<int, unsigned char> GetResources();
    DMAG::Player* GetEastNeighbor();
    DMAG::Player* GetWestNeighbor();

    void SetNeighbors(DMAG::Player* east, DMAG::Player* west);
    void SetWonder(DMAG::Wonder _board);
    void SetId(int id);
};

}
#endif // PLAYER_H
