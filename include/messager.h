//
// Created by jio on 9/13/19.
//

#ifndef INC_7WONDERS_MESSAGER_H
#define INC_7WONDERS_MESSAGER_H

//#include <json>
#include <string>
#include <list>
#include <player.h>
#include <card.h>

namespace DMAG {
    class Messager {

    public:
        // returns the visible game state, to all players
        std::string JSONMessage(std::list<Player> _p, Deck _d);
        // returns the drawn card, should only be evaluated by the current player
        std::string JSONMessage(Card _c);
    };
}


#endif //INC_7WONDERS_MESSAGER_H
