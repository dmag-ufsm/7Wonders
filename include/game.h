#ifndef GAME_H
#define GAME_H

#include <card.h>
#include <player.h>
#include <vector>

namespace DMAG {


class Game
{
private:
    std::vector<Card> deck;
    std::vector<Player> players;
    int num_players;
    int age;
    int round;
    // timer?

public:
    Game(int num_players);
    // ingame?
    // need to think about parameters, game config, etc.
    void NewGame();
    void NextRound();
    void DealCards();
    void DealWonders();
    void ShuffleDeck();
    int GetAge();

};
}

#endif // GAME_H
