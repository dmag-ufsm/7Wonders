#ifndef CARD_H
#define CARD_H

#include <string>

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
			pawnshop,
			baths,
			altar,
			theater,
			aquaduct,
			temple,
			statue,
			courthouse,
			pantheon,
			gardens,
			town_hall,
			palace,
			senate,
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
			stables,
			archery_range,
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
		};
	}

	namespace GUILD{
		enum{
			workers,
			craftsmen,
			traders,
			philosophers,spies,
			strategists,
			ship_owners,
			scientists,
			magistrates,
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
    int _id;
    std::string _name;
    int _value;
    int _type;

public:
    Card();
    Card(int, std::string, int, int);
    std::string GetName();
    int GetValue();
    int GetType();
};

}
#endif // CARD_H
