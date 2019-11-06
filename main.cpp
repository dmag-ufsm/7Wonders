#include <iostream>
#include <cstdlib>
#include <card.h>
#include <player.h>
#include <wonder.h>
//#include <deck.h>
#include <resources.h>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <filer.h>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace std;
using namespace DMAG;

class Game{
	private:
		vector<Player*> player_list;
		unsigned char number_of_players;
		unsigned char era;
		unsigned char turn;
		vector<Wonder> wonders;
		vector<Card> deck[3]; // need to change to Deck deck[3];
		list<Card> discard_pile; // need to change to Deck discard_pile;

		Filer fp;

	public:

		Game(){
			this->number_of_players = 3;
			this->era = 1;
			this->turn = 0;
			//deck[this->era - 1] = Deck(this->era, this->number_of_players);
			//this->discard_pile = Deck();

		}
		// Initiate global variables

		bool InGame(){

			if(turn < 21) // Total number of turns in a game
				return true;

			return false;
		}

		void NextTurn(int actual_player_id){

			// TODO: check if the player has the wonder effects that make
			// possible to play another card in the same round and do it

			turn++;

			// if end of an era (turns 7 14 and 21)
			if (turn % 7 == 0) {
				for (Player* & player : player_list)
					player->Battle(era);

				era++;
				cout << "-----------------------------------------------------------------\n";
				cout << "Nova era: %d\n"<< era << endl;
				// transfer the remaining card in each player's hand to the discarded card list
				for (Player* & player : player_list) {
					vector<Card> cards = player->GetHandCards();
					// cards must be size 1!!
					for (int i = 0; i < cards.size(); i++)
						discard_pile.push_back(cards[i]);
				}

				GiveCards();
			}
			else {
				// needs to be tested

				Player *player, *p1, *neighbor;
				p1 = player = player_list.front();
				vector<Card> neighbor_deck, player_deck = p1->GetHandCards();

				bool clockwise = (era == 1 || era == 3);
				do {
					// Get the neighbor of player p who will receive the cards
					// Keep his deck from being overwritten and lost
					// The neighbor receives the cards from player p
					// The neighbor becomes the player p
					// Continue until you reach the first player again
					neighbor = clockwise ? player->GetEastNeighbor() : player->GetWestNeighbor();
					neighbor_deck = neighbor->GetHandCards();
					neighbor->ReceiveCards(player_deck);
					player = neighbor;
					player_deck = neighbor_deck;
				} while (p1 != player);
			}
		}

		void GiveCards(){
			vector<Card> cards;
			int card_idx = 0;

			random_shuffle(deck[era-1].begin(), deck[era-1].end());


			for (Player* & player : player_list) {
				cards.clear();
				for (int i = 0; i < 7; i++) {
					cards.push_back(deck[era-1][card_idx++]);
				}
				player->ReceiveCards(cards);
			}
		}

		void GiveWonders(){
			bool wonder_availability[7];
			for (int i = 0; i < 7; i++)
				wonder_availability[i] = true;

			for (Player* & player : player_list) {
				while (true) {
					int n = rand() % 14;
					if (wonder_availability[n % 7]) {
						player->SetWonder(wonders[n]);
						wonder_availability[n % 7] = false;
						break;
					}
				}
			}
		}

		void CreateWonders(){
			/* This will need to be reworked. See wonder.h

			   wonders.push_back(Wonder(WONDER_ID::gizah_a, "Gizah A", 0, {}));
			   wonders.push_back(Wonder(WONDER_ID::babylon_a, "Babylon A", 0, {}));
			   wonders.push_back(Wonder(WONDER_ID::olympia_a, "Olympia A", 0, {}));
			   wonders.push_back(Wonder(WONDER_ID::rhodes_a, "Rhodes A", 0, {}));
			   wonders.push_back(Wonder(WONDER_ID::ephesos_a, "Ephesos A", 0, {}));
			   wonders.push_back(Wonder(WONDER_ID::alexandria_a, "Alexandria A", 0, {}));
			   wonders.push_back(Wonder(WONDER_ID::halikarnassos_a, "Halikarnassos A", 0, {}));
			   wonders.push_back(Wonder(WONDER_ID::gizah_b, "Gizah B", 0, {}));
			   wonders.push_back(Wonder(WONDER_ID::babylon_b, "Babylon B", 0, {}));
			   wonders.push_back(Wonder(WONDER_ID::olympia_b, "Olympia B", 0, {}));
			   wonders.push_back(Wonder(WONDER_ID::rhodes_b, "Rhodes B", 0, {}));
			   wonders.push_back(Wonder(WONDER_ID::ephesos_b, "Ephesos B", 0, {}));
			   wonders.push_back(Wonder(WONDER_ID::alexandria_b, "Alexandria B", 0, {}));
			   wonders.push_back(Wonder(WONDER_ID::halikarnassos_b, "Halikarnassos B", 0, {}));
			   */
		}

		void CreateDecks(int era){
			list<Card> cards;

			// TODO: complete the remaining arguments for card: cost, provides, free to and free with

			// Raw Material
			/*
			   cards.push_back(Card(CARD_ID::MATERIALS::lumber_yard, "Lumber Yard", CARD_TYPE::materials, 1, {1, 2, 2, 2, 2}));
			   cards.push_back(Card(CARD_ID::MATERIALS::stone_pit, "Stone Pit", CARD_TYPE::materials, 1, {1, 1, 2, 2, 2}));
			   cards.push_back(Card(CARD_ID::MATERIALS::clay_pool, "Clay Pool", CARD_TYPE::materials, 1, {1, 1, 2, 2, 2}));
			   cards.push_back(Card(CARD_ID::MATERIALS::ore_vein, "Ore Vein", CARD_TYPE::materials, 1, {1, 2, 2, 2, 2}));
			   cards.push_back(Card(CARD_ID::MATERIALS::tree_farm, "Tree Farm", CARD_TYPE::materials, 1, {0, 0, 0, 1, 1}));
			   cards.push_back(Card(CARD_ID::MATERIALS::excavation, "Excavation", CARD_TYPE::materials, 1, {0, 1, 1, 1, 1}));
			   cards.push_back(Card(CARD_ID::MATERIALS::clay_pit, "Clay Pit", CARD_TYPE::materials, 1, {1, 1, 1, 1, 1}));
			   cards.push_back(Card(CARD_ID::MATERIALS::timber_yard, "Timber Yard", CARD_TYPE::materials, 1, {1, 1, 1, 1, 1}));
			   cards.push_back(Card(CARD_ID::MATERIALS::forest_cave, "Forest Cave", CARD_TYPE::materials, 1, {0, 0, 1, 1, 1}));
			   cards.push_back(Card(CARD_ID::MATERIALS::mine, "Mine", CARD_TYPE::materials, 1, {0, 0, 0, 1, 1}));
			   cards.push_back(Card(CARD_ID::MATERIALS::sawmill, "Sawmill", CARD_TYPE::materials, 2, {1, 2, 2, 2, 2}));
			   cards.push_back(Card(CARD_ID::MATERIALS::quarry, "Quarry", CARD_TYPE::materials, 2, {1, 2, 2, 2, 2}));
			   cards.push_back(Card(CARD_ID::MATERIALS::brickyard, "Brickyard", CARD_TYPE::materials, 2, {1, 2, 2, 2, 2}));
			   cards.push_back(Card(CARD_ID::MATERIALS::foundry, "Foundry", CARD_TYPE::materials, 2, {1, 2, 2, 2, 2}));
			// Manufactured Good
			cards.push_back(Card(CARD_ID::MANUFACTURED::loom, "Loom", CARD_TYPE::manufactured, 1, {1, 1, 1, 2, 2}));
			cards.push_back(Card(CARD_ID::MANUFACTURED::glassworks, "Glassworks", CARD_TYPE::manufactured, 1, {1, 1, 1, 2, 2}));
			cards.push_back(Card(CARD_ID::MANUFACTURED::press, "Press", CARD_TYPE::manufactured, 1, {1, 1, 1, 2, 2}));
			cards.push_back(Card(CARD_ID::MANUFACTURED::loom, "Loom", CARD_TYPE::manufactured, 2, {1, 1, 2, 2, 2}));
			cards.push_back(Card(CARD_ID::MANUFACTURED::glassworks, "Glassworks", CARD_TYPE::manufactured, 2, {1, 1, 2, 2, 2}));
			cards.push_back(Card(CARD_ID::MANUFACTURED::press, "Press", CARD_TYPE::manufactured, 2, {1, 1, 2, 2, 2}));
			// Civilian Structure
			cards.push_back(Card(CARD_ID::CIVILIAN::altar, "Altar", CARD_TYPE::civilian, 1, {1, 1, 2, 2, 2}));
			cards.push_back(Card(CARD_ID::CIVILIAN::theater, "Theater", CARD_TYPE::civilian, 1, {1, 1, 1, 2, 2}));
			cards.push_back(Card(CARD_ID::CIVILIAN::pawnshop, "Pawnshop", CARD_TYPE::civilian, 1, {0, 1, 1, 1, 2}));
			cards.push_back(Card(CARD_ID::CIVILIAN::baths, "Baths", CARD_TYPE::civilian, 1, {1, 1, 1, 1, 2}));
			cards.push_back(Card(CARD_ID::CIVILIAN::temple, "Temple", CARD_TYPE::civilian, 2, {1, 1, 1, 2, 2}));
			cards.push_back(Card(CARD_ID::CIVILIAN::courthouse, "Courthouse", CARD_TYPE::civilian, 2, {1, 1, 2, 2, 2}));
			cards.push_back(Card(CARD_ID::CIVILIAN::statue, "Statue", CARD_TYPE::civilian, 2, {1, 1, 1, 1, 2}));
			cards.push_back(Card(CARD_ID::CIVILIAN::aqueduct, "Aqueduct", CARD_TYPE::civilian, 2, {1, 1, 1, 1, 2}));
			cards.push_back(Card(CARD_ID::CIVILIAN::gardens, "Gardens", CARD_TYPE::civilian, 3, {1, 2, 2, 2, 2}));
			cards.push_back(Card(CARD_ID::CIVILIAN::town_hall, "Town Hall", CARD_TYPE::civilian, 3, {1, 1, 2, 3, 3}));
			cards.push_back(Card(CARD_ID::CIVILIAN::senate, "Senate", CARD_TYPE::civilian, 3, {1, 1, 2, 2, 2}));
			cards.push_back(Card(CARD_ID::CIVILIAN::pantheon, "Pantheon", CARD_TYPE::civilian, 3, {1, 1, 1, 2, 2}));
			cards.push_back(Card(CARD_ID::CIVILIAN::palace, "Palace", CARD_TYPE::civilian, 3, {1, 1, 1, 1, 2}));
			// Commercial Structure
			cards.push_back(Card(CARD_ID::COMMERCIAL::tavern, "Tavern", CARD_TYPE::commercial, 1, {0, 1, 2, 2, 3}));
			cards.push_back(Card(CARD_ID::COMMERCIAL::east_trading_post, "East Trading Post", CARD_TYPE::commercial, 1, {1, 1, 1, 1, 2}));
			cards.push_back(Card(CARD_ID::COMMERCIAL::west_trading_post, "West Trading Post", CARD_TYPE::commercial, 1, {1, 1, 1, 1, 2}));
			cards.push_back(Card(CARD_ID::COMMERCIAL::marketplace, "Marketplace", CARD_TYPE::commercial, 1, {1, 1, 1, 2, 2}));
			cards.push_back(Card(CARD_ID::COMMERCIAL::forum, "Forum", CARD_TYPE::commercial, 2, {1, 1, 1, 2, 3}));
			cards.push_back(Card(CARD_ID::COMMERCIAL::caravansery, "Caravansery", CARD_TYPE::commercial, 2, {1, 1, 2, 3, 3}));
			cards.push_back(Card(CARD_ID::COMMERCIAL::vineyard, "Vineyard", CARD_TYPE::commercial, 2, {1, 1, 1, 2, 2}));
			cards.push_back(Card(CARD_ID::COMMERCIAL::bazar, "Bazar", CARD_TYPE::commercial, 2, {0, 1, 1, 1, 2}));
			cards.push_back(Card(CARD_ID::COMMERCIAL::haven, "Haven", CARD_TYPE::commercial, 3, {1, 2, 2, 2, 2}));
			cards.push_back(Card(CARD_ID::COMMERCIAL::lighthouse, "Lighthouse", CARD_TYPE::commercial, 3, {1, 1, 1, 2, 2}));
			cards.push_back(Card(CARD_ID::COMMERCIAL::chamber_of_commerce, "Chamber of Commerce", CARD_TYPE::commercial, 3, {0, 1, 1, 2, 2}));
			cards.push_back(Card(CARD_ID::COMMERCIAL::arena, "Arena", CARD_TYPE::commercial, 3, {1, 1, 2, 2, 3}));
			// Military Structure
			cards.push_back(Card(CARD_ID::MILITARY::stockade, "Stockade", CARD_TYPE::military, 1, {1, 1, 1, 1, 2}));
			cards.push_back(Card(CARD_ID::MILITARY::barracks, "Barracks", CARD_TYPE::military, 1, {1, 1, 2, 2, 2}));
			cards.push_back(Card(CARD_ID::MILITARY::guard_tower, "Guard Tower", CARD_TYPE::military, 1, {1, 2, 2, 2, 2}));
			cards.push_back(Card(CARD_ID::MILITARY::walls, "Walls", CARD_TYPE::military, 2, {1, 1, 1, 1, 2}));
			cards.push_back(Card(CARD_ID::MILITARY::training_ground, "Training Ground", CARD_TYPE::military, 2, {0, 1, 1, 2, 3}));
			cards.push_back(Card(CARD_ID::MILITARY::stables, "Stables", CARD_TYPE::military, 2, {1, 1, 2, 2, 2}));
			cards.push_back(Card(CARD_ID::MILITARY::archery_range, "Archery Range", CARD_TYPE::military, 2, {1, 1, 1, 2, 2}));
			cards.push_back(Card(CARD_ID::MILITARY::fortifications, "Fortifications", CARD_TYPE::military, 3, {1, 1, 1, 1, 2}));
			cards.push_back(Card(CARD_ID::MILITARY::circus, "Circus", CARD_TYPE::military, 3, {0, 1, 2, 3, 3}));
			cards.push_back(Card(CARD_ID::MILITARY::arsenal, "Arsenal", CARD_TYPE::military, 3, {1, 1, 1, 1, 2}));
			cards.push_back(Card(CARD_ID::MILITARY::siege_workshop, "Siege Workshop", CARD_TYPE::military, 3, {1, 1, 2, 2, 2}));
			// Scientific Structure
			cards.push_back(Card(CARD_ID::SCIENTIFIC::apothecary, "Apothecary", CARD_TYPE::scientific, 1, {1, 1, 2, 2, 2}));
			cards.push_back(Card(CARD_ID::SCIENTIFIC::workshop, "Workshop", CARD_TYPE::scientific, 1, {1, 1, 1, 1, 2}));
			cards.push_back(Card(CARD_ID::SCIENTIFIC::scriptorium, "Scriptorium", CARD_TYPE::scientific, 1, {1, 2, 2, 2, 2}));
			cards.push_back(Card(CARD_ID::SCIENTIFIC::dispensary, "Dispensary", CARD_TYPE::scientific, 2, {1, 2, 2, 2, 2}));
			cards.push_back(Card(CARD_ID::SCIENTIFIC::laboratory, "Laboratory", CARD_TYPE::scientific, 2, {1, 1, 2, 2, 2}));
			cards.push_back(Card(CARD_ID::SCIENTIFIC::library, "Library", CARD_TYPE::scientific, 2, {1, 1, 1, 2, 2}));
			cards.push_back(Card(CARD_ID::SCIENTIFIC::school, "School", CARD_TYPE::scientific, 2, {1, 1, 1, 1, 2}));
			cards.push_back(Card(CARD_ID::SCIENTIFIC::lodge, "lodge", CARD_TYPE::scientific, 3, {1, 1, 1, 2, 2}));
			cards.push_back(Card(CARD_ID::SCIENTIFIC::observatory, "Observatory", CARD_TYPE::scientific, 3, {1, 1, 1, 1, 2}));
			cards.push_back(Card(CARD_ID::SCIENTIFIC::university, "University", CARD_TYPE::scientific, 3, {1, 1, 2, 2, 2}));
			cards.push_back(Card(CARD_ID::SCIENTIFIC::academy, "Academy", CARD_TYPE::scientific, 3, {1, 1, 1, 1, 2}));
			cards.push_back(Card(CARD_ID::SCIENTIFIC::study, "Study", CARD_TYPE::scientific, 3, {1, 1, 2, 2, 2}));
			// Guild
			cards.push_back(Card(CARD_ID::GUILD::workers, "Workers Guild", CARD_TYPE::guild, 3, {0, 0, 0, 0, 0}));
			cards.push_back(Card(CARD_ID::GUILD::craftsmens, "Craftsmens Guild", CARD_TYPE::guild, 3, {0, 0, 0, 0, 0}));
			cards.push_back(Card(CARD_ID::GUILD::traders, "Traders Guild", CARD_TYPE::guild, 3, {0, 0, 0, 0, 0}));
			cards.push_back(Card(CARD_ID::GUILD::philosophers, "Philosophers Guild", CARD_TYPE::guild, 3, {0, 0, 0, 0, 0}));
			cards.push_back(Card(CARD_ID::GUILD::spies, "Spies Guild", CARD_TYPE::guild, 3, {0, 0, 0, 0, 0}));
			cards.push_back(Card(CARD_ID::GUILD::magistrates, "Magistrates Guild", CARD_TYPE::guild, 3, {0, 0, 0, 0, 0}));
			cards.push_back(Card(CARD_ID::GUILD::shipowners, "Shipowners Guild", CARD_TYPE::guild, 3, {0, 0, 0, 0, 0}));
			cards.push_back(Card(CARD_ID::GUILD::strategists, "Strategists Guild", CARD_TYPE::guild, 3, {0, 0, 0, 0, 0}));
			cards.push_back(Card(CARD_ID::GUILD::scientists, "Scientists Guild", CARD_TYPE::guild, 3, {0, 0, 0, 0, 0}));
			cards.push_back(Card(CARD_ID::GUILD::builders, "Builders Guild", CARD_TYPE::guild, 3, {0, 0, 0, 0, 0}));

			for (Card & card : cards) {
				int amount = card.GetAmount(this->number_of_players);
				for (int i = 0; i < amount; i++) {
					deck[card.GetEra()-1].push_back(card);
				}
			}
			*/

				// TODO: add guild cards to decks
		}


		int NewGame(int _players){
			this->number_of_players = _players;
			Player *p;

			for(int i = 0; i < this->number_of_players; i++){
				p = new Player();
				p->SetId(i);
				player_list.push_back(p);
			}

			for(int i = 0; i < this->number_of_players; i++){
				if(i == 0){
					player_list[i]->SetNeighbors(player_list[this->number_of_players -1], player_list[i+1]);
				}else if(i == this->number_of_players -1){
					player_list[i]->SetNeighbors(player_list[i-1], player_list[0]);
				}else{
					player_list[i]->SetNeighbors(player_list[i-1], player_list[i+1]);
				}
			}

			//CreateDecks();
			//GiveWonders();
			//GiveCards();

			return 0;
		}

		void Init(){
			CreateWonders();
			fp.Init(3);

		}

		void Close(){

			// deallocate memory
		}
		void Loop(){

			json json_object;
			while(InGame()){
				while(!fp.ArePlayersReady());
				for(int i = 0; i < number_of_players; i++){
					json_object = fp.ReadMessages(i);
					cout << json_object << endl;
					// handle command inside json_object
					if(json_object["command"] == "build_structure"){
						player_list[i]->BuildStructure(Card()); // find card before calling this
					}
				}
				//g.NextTurn();

				//show info

				//get commands

				//calculate stuff

				//end game?
			}
		}

};


int main()
{
	Game g;
	g.Init();
	g.NewGame(3);

	//    g.NextTurn(p, 0); //this function is not completed
	g.Loop();
	g.Close();

	return 0;
}
