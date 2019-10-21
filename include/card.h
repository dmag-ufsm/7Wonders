#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>

namespace CARD_ID{
    namespace MATERIALS{
        enum{
            lumber_yard,
            stone_pit,
            clay_pool,
            ore_vein,
            tree_farm,
            excavation,
            clay_pit,
            timber_yard,
            forest_cave,
            mine,
            sawmill,
            quarry,
            brickyard,
            foundry,

        };
    }

    namespace MANUFACTURED{
        enum{
            loom,
            glassworks,
            press,
        };
    }

    namespace CIVILIAN{
        enum{
            altar,
            theater,
            pawnshop,
            baths,
            temple,
            courthouse,
            statue,
            aqueduct,
            gardens,
            town_hall,
            senate,
            pantheon,
            palace,
        };
    }

    namespace COMMERCIAL{
        enum{
            tavern,
            east_trading_post,
            west_trading_post,
            marketplace,
            forum,
            caravansery,
            vineyard,
            bazar,
            haven,
            lighthouse,
            chamber_of_commerce,
            arena,
        };
    }

    namespace MILITARY{
        enum{
            stockade,
            barracks,
            guard_tower,
            walls,
            training_ground,
            stables,
            archery_range,
            fortifications,
            circus,
            arsenal,
            siege_workshop,
        };
    }

    namespace SCIENTIFIC{
        enum{
            apothecary,
            workshop,
            scriptorium,
            dispensary,
            laboratory,
            library,
            school,
            lodge,
            observatory,
            university,
            academy,
            study,
        };
    }

    namespace GUILD{
        enum{
            workers,
            craftsmens,
            traders,
            philosophers,
            spies,
            magistrates,
            shipowners,
            strategists,
            scientists,
            builders,
        };
    }

}


namespace CARD_TYPE {
enum type{
    materials,
    manufactured,
    civilian,
    commercial,
    military,
    scientific,
    guild,
    };
}

namespace DMAG {

class Card
{
private:
    int id;
    std::string name;
    int type;
    int era;
    int amount[5];

public:
    Card();
    Card(int id, std::string name, int type, int era, std::vector<int> amount);
    [[nodiscard]] std::string GetName() const;
    [[nodiscard]] int GetId() const;
    int GetType() const;
    int GetEra() const;
    int GetAmount(int) const;
};

}
#endif // CARD_H
