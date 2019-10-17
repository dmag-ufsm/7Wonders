#include <iostream>
#include <card.h>
#include <player.h>
#include <list>

using namespace std;
using namespace DMAG;


class Game{
	private:
	list<Player> player_list;

	
	public:
	Game(){
	}
	// Initiate global variables

	void Init(int _players){
		for(int i = 0; i<_players; i++){
			player_list.push_back(Player());
		}


		return;
	}

	void Close(){

		// deallocate memory
	}
	void Loop(){

		// create connection with RabbitMQ

		//show info

		//get commands

		//calculate stuff

		//end game?
	}
};


int main()
{
	Game g;
	g.Init(3);
	g.Loop();
	g.Close();

	return 0;
}


