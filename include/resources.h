//
// Created by Emilio on 20/10/2019.
//

#ifndef INC_7WONDERS_RESOURCES_H
#define INC_7WONDERS_RESOURCES_H

namespace RESOURCE {
    namespace MATERIALS {
        enum {
            lumber,
            ore,
            clay,
            stone
        };
    }
    namespace MANUFACTURE {
        enum {
            loom,
            glassworks,
            press
        };
    }
    namespace SCIENCE {
        enum {
            engineering, // cogs
            navigation,  // compass
            writing      // tablet
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

#endif //INC_7WONDERS_RESOURCES_H
