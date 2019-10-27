#ifndef WONDER_H
#define WONDER_H

#include <resources.h>
#include <string>
#include <vector>
#include <map>

namespace WONDER_ID{
    enum{
        // 0-6 side A
        gizah_a,
        babylon_a,
        olympia_a,
        rhodos_a,
        ephesos_a,
        alexandria_a,
        halikarnassos_a,
        // 7-13 side B
        gizah_b,
        babylon_b,
        olympia_b,
        rhodos_b,
        ephesos_b,
        alexandria_b,
        halikarnassos_b,
    };
}

// This will probably change in the future, but nevertheless it's good to see every effect possible.
namespace EFFECT{
    enum{
        vp, // Victory points;

        // Once per age, a player can construct a building from his hand for free.
        hand_build_for_free,     // olympia_a
        // The player can look at all cards discarded since the beginning of the game,
        // pick one and build it for free (happens at the end of the turn).
        discard_build_for_free,  // halikarnassos_a, halikarnassos_b stage 3
        // The player adds two shields to his total at each conflict resolution.
        add_shields,             // rhodes_a
        // The player chooses one of the 4 raw resources to receive for free at each turn (untradable).
        choose_raw,              // alexandria_a, alexandria_b stage 1
        // The player adds 9 gold coins to his treasury immediately after the stage is constructed.
        add_coins,               // ephesus_a
        // The players chooses one of the 3 scientific symbols to get for free at the end of the game.
        add_science,             // babylon_a, babylon_b stage 3

        // Effects exclusive to Side B:
        add_shield_vp_coin,      // rhodes_b, both stages (1-3-3 and 1-4-4)
        choose_manuf,            // alexandria_b, stage 2
        add_vp_coin,             // ephesus_b, all stages (2-4, 3-4 and 5-4)
        // The player can choose to play the seventh card of an Age instead of discarding it.
        play_seventh_card,       // babylon_b, stage 2
        // The player can buy resources for 1 coin instead of 2 from neighbours.
        buy_raw_cheap,           // olympia_b, stage 1
        // The player can copy a Guild from his neighbours (at the end of the game).
        copy_guild,              //olympia_b, stage 3
        add_vp_discard,          // halikarnassos_b, stages 1 and 2 (2-discard and 1-discard)

        // obs.: Gizeh only gives VP on both A and B sides.
    };
}

namespace DMAG {

class Player; // Forward declaration to solve circular dependency issue.

class Wonder
{
protected:
    int id;
    int production;
    std::string name;

    // Idea:
    // the list of effects will be basically a list of X values that represent the enum above.
    // (X being the number of stages that will be equal to the number of effects)...
    // Example: gizeh_b       ->  (vp, vp, vp, vp);
    //          alexandria_a  ->  (vp, add_raw_resource, vp)
    // I truly hope someone can think of something better!
    std::vector<int> effects;
    int stage;
    int wonder_points;

    std::map<int, unsigned char> cost{
        { RESOURCE::wood, 0 },
        { RESOURCE::ore, 0 },
        { RESOURCE::clay, 0 },
        { RESOURCE::stone, 0 },
        { RESOURCE::cloth, 0 },
        { RESOURCE::glass, 0 },
        { RESOURCE::papyrus, 0 },
        { RESOURCE::gear, 0 },
        { RESOURCE::compass, 0 },
        { RESOURCE::tablet, 0 }
    };

public:
    Wonder();
    std::string GetName() const;
    int GetType() const;
    int GetProduction() const;
    int GetStage() const;
    int GetWonderPoints() const;
    std::vector<int> GetEffects() const;
    virtual bool AddStage(Player* p);
};

// I went with snake_case because it's clearer in this case.
// e.g. Olympia_a vs OlympiaA
class Gizah_a : public Wonder
{
public:
    Gizah_a();
    bool AddStage(Player* p) override;
};

class Gizah_b : public Wonder
{
public:
    Gizah_b();
    bool AddStage(Player* p) override;
};

class Babylon_a : public Wonder
{
public:
    Babylon_a();
    bool AddStage(Player* p) override;
};

class Babylon_b : public Wonder
{
public:
    Babylon_b();
    bool AddStage(Player* p) override;
};

class Olympia_a : public Wonder
{
public:
    Olympia_a();
    bool AddStage(Player* p) override;
};

class Olympia_b : public Wonder
{
public:
    Olympia_b();
    bool AddStage(Player* p) override;
};

class Rhodos_a : public Wonder
{
public:
    Rhodos_a();
    bool AddStage(Player* p) override;
};

class Rhodos_b : public Wonder
{
public:
    Rhodos_b();
    bool AddStage(Player* p) override;
};

class Ephesos_a : public Wonder
{
public:
    Ephesos_a();
    bool AddStage(Player* p) override;
};

class Ephesos_b : public Wonder
{
public:
    Ephesos_b();
    bool AddStage(Player* p) override;
};

class Alexandria_a : public Wonder
{
public:
    Alexandria_a();
    bool AddStage(Player* p) override;
};

class Alexandria_b : public Wonder
{
public:
    Alexandria_b();
    bool AddStage(Player* p) override;
};

class Halikarnassos_a : public Wonder
{
public:
    Halikarnassos_a();
    bool AddStage(Player* p) override;
};

class Halikarnassos_b : public Wonder
{
public:
    Halikarnassos_b();
    bool AddStage(Player* p) override;
};

}

#endif // WONDER_H
