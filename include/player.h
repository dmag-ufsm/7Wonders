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
    DMAG::Wonder board;
    std::vector<DMAG::Card> cards_hand;
    std::vector<DMAG::Card> cards_played;
    signed char victory_tokens, defeat_tokens;
    unsigned char victory_points;

    bool play_seventh;         // Can the player play the seventh card or not? (Wonder effect)
    bool wonder_raw_cheap;     // Can the player buy raw resources from neighbors for 1 coin? (Wonder effect)
    bool raw_cheap_east;       // Can the player buy raw resources from eastern neighbor for 1 coin? (East Trading Post)
    bool raw_cheap_west;       // Can the player buy raw resources from western neighbor for 1 coin? (West Trading Post)
    bool manuf_cheap;          // Can the player buy manufactured resources from neighbors for 1 coin? (Marketplace)
    int raw_extra;             // The player has an additional raw material. (Caravansery and Alexandria A)
    int manuf_extra;           // The player has an additional manufactured good. (Forum and Alexandria B)
    int sci_extra;             // The player has an additional scientific piece. (Scientific Guild and Babylon A and B)

    // Key needs to be int because the underlying type in enums is int
    std::map<int, unsigned char> resources{
        { RESOURCE::wood, 0 },
        { RESOURCE::ore, 0 },
        { RESOURCE::clay, 0 },
        { RESOURCE::stone, 0 },
        { RESOURCE::loom, 0 },
        { RESOURCE::glass, 0 },
        { RESOURCE::papyrus, 0 },
        { RESOURCE::gear, 0 },
        { RESOURCE::compass, 0 },
        { RESOURCE::tablet, 0 },
        { RESOURCE::coins, 3 },
        { RESOURCE::shields, 0 }
    };

    DMAG::Player* player_east;
    DMAG::Player* player_west;

public:
    Player();

    // Card-related:
    bool BuildWonder(DMAG::Card c);
    bool BuildStructure(DMAG::Card c, std::vector<DMAG::Card> cards, bool _free_card);
    bool CheckFreeCard(DMAG::Card c);
    std::vector<DMAG::Card> GetHandCards();
    void ReceiveCards(std::vector<DMAG::Card> _cards_hand);
    void Discard();
    int AmountOfType(int card_type);

    // Resource-related:
    bool BuyResource(int resource, int quant);
    void AddResource(int resource, int quant);
    bool HasEnoughResource(int resource, int quant);

    // Battle-related:
    void Battle(int age);

    // Scoring-related:
    int CalculateCivilianScore();
    int CalculateCommercialScore();
    int CalculateGuildScore();
    int CalculateScientificScore();
    int CalculateScore();

    // Wonder-related effects:
    bool ChooseExtraManuf(int resource);   // At the end of the game.
    bool ChooseExtraScience(int resource); // At the end of the game.
    bool ChooseExtraRaw(int resource);     // Once per turn.
    void CanPlaySeventh();
    void CanBuyRawCheap();
    bool BuildDiscardFree(DMAG::Card c, std::vector<DMAG::Card> discard_pile); // At the end of the turn after the stage was built.
    bool BuildHandFree(DMAG::Card c); // Once per Age.
    bool CopyGuild(DMAG::Card c, int side); // At the end of the game.

    // Getters:
    int GetId();
    int GetShields();
    DMAG::Wonder GetBoard();
    int GetDefeatTokens();
    std::map<int, unsigned char> GetResources();
    DMAG::Player* GetEastNeighbor();
    DMAG::Player* GetWestNeighbor();

    // Setters:
    void SetNeighbors(DMAG::Player* east, DMAG::Player* west);
    void SetWonder(DMAG::Wonder _board);
    void SetId(int id);
};

}
#endif // PLAYER_H
