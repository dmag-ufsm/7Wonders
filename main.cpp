#include <iostream>
#include <card.h>
#include <player.h>
#include <deck.h>

using namespace std;
using namespace DMAG;

void game_loop(Deck _d){
    //show info

    while(!_d.IsEmpty()){
       cout << _d.GetTop().GetName() << endl;
    }
    //get commands

    //calculate stuff

    //end game?
}

int main(int argc, char **argv)
{
    Deck d = Deck();
    game_loop(d);
    return 0;
}


