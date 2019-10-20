#include <iostream>
#include <card.h>
#include <player.h>
#include <list>
#include <deck.h>
#include <resources.h>

using namespace std;
using namespace DMAG;


class Game{
	private:
		list<Player> player_list;
		unsigned char number_of_players;
		unsigned char era;
		unsigned char turn;
		Deck deck[3]; //3 eras, so 3 decks
		Deck discard_pile;



	public:

		Game(){
			this->number_of_players = 3;
			this->era = 1;
			this->turn = 0;
			deck[this->era - 1] = Deck(this->era, this->number_of_players);
			this->discard_pile = Deck();
		}
		// Initiate global variables

		bool InGame(){

			if(turn < 21) // Total number of turns in a game
				return true;

			return false;
		}

		void NextTurn(){
		}

		void GiveCards(){
		}

		void GiveWonders(){
		}


		/*
		 * This method will write to the file for other
		 * processes to read, and will return an execution
		 * code.
		 */
		int WriteToFile(){
			return 0;
		}

		/*
		 * The return code could be the number of read files,
		 * or the number of read bytes.
		 */
		int ReadFromFile(){
			return 0;
		}

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


