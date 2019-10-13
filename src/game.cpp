#include "../include/game.h"

namespace DMAG {

Game::Game(int num_players)
{
    this->num_players = num_players;
    this->age = 1;
    this->round = 1;
}

int Game::GetAge(){
    return this->age;
}

}
