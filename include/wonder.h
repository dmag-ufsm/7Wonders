#ifndef WONDER_H
#define WONDER_H

#include <string>
#include <list>

namespace WONDER_ID{
    enum{
        // 0-6 side A
        gizah_a,
        rhodos_a,
        alexandria_a,
        babylon_a,
        ephesus_a,
        olympia_a,
        halikarnassos_a,
        // 7-13 side B
        gizah_b,
        rhodos_b,
        alexandria_b,
        babylon_b,
        ephesus_b,
        olympia_b,
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
        add_raw_resource,        // alexandria_a, alexandria_b stage 1
        // The player adds 9 gold coins to his treasury immediately after the stage is constructed.
        add_coins,               // ephesus_a
        // The players chooses one of the 3 scientific symbols to get for free at the end of the game.
        add_scientific,          // babylon_a, babylon_b stage 3

        // Effects exclusive to Side B:
        add_shield_vp_coin,      // rhodes_b, both stages (1-3-3 and 1-4-4)
        add_manuf_resource,      // alexandria_b, stage 2
        add_vp_coin,             // ephesus_b, all stages (2-4, 3-4 and 5-4)
        // The player can choose to play the seventh card of an Age instead of discarding it.
        play_seventh_card,       // babylon_b, stage 2
        // The player can buy resources for 1 coin instead of 2 from neighbours.
        buy_raw_resource_cheap,  // olympia_b, stage 1
        // The player can copy a Guild from his neighbours (at the end of the game).
        copy_guild,              //olympia_b, stage 3
        add_vp_discard,          // halikarnassos_b, stages 1 and 2 (2-discard and 1-discard)

        // obs.: Gizeh only gives VP on both A and B sides.
    };
}

namespace DMAG {

class Wonder
{
private:
    int id;
    int production;
    std::string name;

    // Idea:
    // the list of effects will be basically a list of X values that represent the enum above.
    // (X being the number of stages that will be equal to the number of effects)...
    // Example: gizeh_b       ->  (vp, vp, vp, vp);
    //          alexandria_a  ->  (vp, add_raw_resource, vp)
    // I truly hope someone can think of something better!
    std::list<int> effects;

    int stage;

public:
    Wonder();
    // id, name, produção, list of effects?
    Wonder(int, std::string, int, std::list<int>);
    std::string GetName() const;
    int GetProduction() const;
    int GetStage() const;
    std::list<int> GetEffects() const;
    void AddStage();
    // StageResources()
};

}
#endif // WONDER_H
