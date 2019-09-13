#include <iostream>
#include <card.h>
#include <player.h>
#include <deck.h>
#include <amqpcpp.h>

using namespace std;
using namespace DMAG;

void game_loop(Deck _d){

    // create connection with RabbitMQ


    //show info

    while(!_d.IsEmpty()){
       cout << _d.GetTop().GetName() << endl;
    }
    //get commands

    //calculate stuff

    //end game?
}

int main()
{
    Deck d = Deck();
    game_loop(d);
    return 0;
}


