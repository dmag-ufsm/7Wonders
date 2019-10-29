#ifndef PLAYER_H
#define PLAYER_H

#include <card.h>
#include <wonder.h>
#include <resources.h>
#include <vector>
#include <list>
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
    bool play_seventh; // Can the player play the seventh card or not? (Wonder effect)
    bool raw_cheap; // Can the player buy raw resources from neighbors for 1 coin? (Wonder effect)

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
    void BuildWonder(DMAG::Card c);
    void BuildStructure(DMAG::Card c);
    void BuildGuild();
    std::vector<DMAG::Card> GetHandCards();
    void ReceiveCards(std::vector<DMAG::Card> _cards_hand);
    DMAG::Card Discard();
    int BuyResource();
    void AddResource(int resource, int quant);
    void AddShield(int quant);
    void Battle(int age);

    int CalculateCivilianScore();
    int CalculateCommercialScore();
    int CalculateGuildScore();
    int CalculateScientificScore();
    int CalculateScore();

    // Wonder-specific effects:
    void ChooseExtraManuf(int resource);   // At the end of the game.
    void ChooseExtraScience(int resource); // At the end of the game.
    void ChooseExtraRaw(int resource);     // Once per turn.
    void CanPlaySeventh();
    void CanBuyRawCheap();
    void BuildDiscardFree(DMAG::Card c, std::list<DMAG::Card> discard_pile); // At the end of the turn after the stage was built.
    void BuildHandFree(DMAG::Card c); // Once per Age.
    void CopyGuild(DMAG::Card c); // At the end of the game.

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
