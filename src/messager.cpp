//
// Created by jio on 9/13/19.
//

#include <messager.h>
#include <string>
#include <deck.h>
#include <player.h>
#include <card.h>
#include <list>

namespace DMAG{

    /*
     * @params: _p: the list of players in the game
     * _d: the deck, as is. Will reveal the top discarded card.
     * @return: the string-ed json message, to broadcast.
     */
    std::string Messager::JSONMessage(std::list<Player> _p, Deck _d) {
        return "";
    }

    /*
     * @params: _c: the drawn card
     * @return: the string-ed json message, should not be evaluated by
     * the other players, only the current one.
     */
    std::string Messager::JSONMessage(DMAG::Card _c) {
        return "";
    }
}