#include <iostream>
#include <card.h>
#include <player.h>
#include <list>

using namespace std;
using namespace DMAG;

// define global variables
list<Player> player_list;


// Initiate global variables

void game_init(int _players){
    for(int i = 0; i<_players; i++){
        player_list.push_back(Player());
    }


    return;
}

void game_close(){

    // deallocate memory
}
void game_loop(){

    // create connection with RabbitMQ

    //show info

    //get commands

    //calculate stuff

    //end game?
}

int main()
{
   // game_init(3);
   // game_loop();
   // game_close();

    return 0;
}


