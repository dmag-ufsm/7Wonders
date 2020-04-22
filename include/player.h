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
    DMAG::Wonder *board;
    std::vector<DMAG::Card> cards_hand;
    std::vector<DMAG::Card> cards_played;
    std::vector<DMAG::Card> cards_playable;
    int victory_tokens, defeat_tokens;
    unsigned int victory_points;

    bool play_seventh;         // Can the player play the seventh card or not? (Wonder effect)
    bool wonder_raw_cheap;     // Can the player buy raw resources from neighbors for 1 coin? (Wonder effect)
    bool raw_cheap_east;       // Can the player buy raw resources from eastern neighbor for 1 coin? (East Trading Post)
    bool raw_cheap_west;       // Can the player buy raw resources from western neighbor for 1 coin? (West Trading Post)
    bool manuf_cheap;          // Can the player buy manufactured resources from neighbors for 1 coin? (Marketplace)
    bool free_card_once;       // EXTRA check whether the player can build a card for free once per Age or not.
    bool discard_free;         // Checks if the player can build an extra card from the discard pile.

    bool raw_extra;            // The player has an additional raw material. (Caravansery and Alexandria A)
    bool manuf_extra;          // The player has an additional manufactured good. (Forum and Alexandria B)
    int sci_extra;             // The player has an additional scientific piece. (Scientific Guild and Babylon A and B)

    // Possible values:
    // -> -1 (not used)
    // -> RESOURCE code (from resources.h)
    int used_tree_farm;
    int used_forest_cave;
    int used_timber_yard;
    int used_excavation;
    int used_mine;
    int used_clay_pit;
    int used_forum;
    int used_caravansery;

    // Key needs to be int because the underlying type in enums is int
    std::map<int, int> resources{
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
    std::vector<DMAG::Card> GetPlayedCards();
    void ReceiveCards(std::vector<DMAG::Card> _cards_hand);
    void Discard(DMAG::Card c);
    int AmountOfType(int card_type);
    bool AvailableCard(int card_id, int resource);
    bool HasPlayedCard(DMAG::Card c);
    std::vector<DMAG::Card> GetPlayableCards();
    void ResetUsed(bool decrement);

    // Resource-related:
    bool ProduceResource(int resource, int quant);
    bool BuyResource(int resource, int quant);
    void AddResource(int resource, int quant);
    int IncrementOnDemand(int resource, int needed, bool is_neighbor);
    void DecrementUsed();
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
    bool CopyGuild(); // At the end of the game.
    bool PlaySeventh();
    void FreeCardOnce(bool flag);
    void DiscardFree(bool flag);

    // Getters:
    int GetId();
    int GetShields();
    DMAG::Wonder* GetBoard();
    int GetDefeatTokens();
    std::map<int, int> GetResources();
    DMAG::Player* GetEastNeighbor();
    DMAG::Player* GetWestNeighbor();

    // Setters:
    void SetNeighbors(DMAG::Player* east, DMAG::Player* west);
    void SetWonder(DMAG::Wonder *_board);
    void SetId(int id);
    void SetResources(std::map<int, int> r);
};

}
#endif // PLAYER_H
