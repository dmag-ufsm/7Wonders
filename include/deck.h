//
// Created by Emilio on 20/10/2019.
//

#ifndef INC_7WONDERS_DECK_H
#define INC_7WONDERS_DECK_H

#include "card.h"

namespace DMAG {
    class Deck: public std::list<Card> {
        public:
            Deck();
            Deck(unsigned char age, unsigned char number_of_players);
    };
}

#endif //INC_7WONDERS_DECK_H
