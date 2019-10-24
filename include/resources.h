//
// Created by Emilio on 20/10/2019.
//

#ifndef INC_7WONDERS_RESOURCES_H
#define INC_7WONDERS_RESOURCES_H

namespace RESOURCE {
    enum {
        wood,
        ore,
        clay,
        stone,

        cloth,
        glass,
        papyrus,

        gear,
        compass,
        tablet,

        coins
    };
}

/* I decided to simplify things a little bit so it'll work with a single map with all the needed resources.
* I hope you're okay with that.

namespace RESOURCE {
namespace MATERIALS {
    enum {
        wood,
        ore,
        clay,
        stone
    };
}
namespace MANUFACTURE {
    enum {
        cloth,
        glass,
        papyrus
    };
}
namespace SCIENCE {
    enum {
        gear,
        compass,
        tablet
    };
}
namespace OTHER {
    enum {
        coin,
        victory,
        battle
    };
}
}
*/

#endif //INC_7WONDERS_RESOURCES_H
