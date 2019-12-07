#include <iostream>
#include <cstdlib>
#include <card.h>
#include <player.h>
#include <wonder.h>
#include <resources.h>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <filer.h>
#include <algorithm>
#include <nlohmann/json.hpp>

#define NUM_PLAYERS 3

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
        vector<Card> discard_pile; // need to change to Deck discard_pile;

        Filer fp;

    public:

        Card GetCardByName(std::string name){
            for(int i = 0; i < 3; ++i){
                for(auto c: deck[i]){
                    if(c.GetName().compare(name) == 0){
                        return c;
                    }
                }
            }
            return Card(0, "not found", 0, 0, 0, std::vector<int>(), std::vector<int>());
        }

        Game(){
            this->number_of_players = NUM_PLAYERS;
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

            wonders.push_back(Gizah_a());
            wonders.push_back(Babylon_a());
            wonders.push_back(Olympia_a());
            wonders.push_back(Rhodos_a());
            wonders.push_back(Ephesos_a());
            wonders.push_back(Alexandria_a());
            wonders.push_back(Halikarnassos_a());
            wonders.push_back(Gizah_b());
            wonders.push_back(Babylon_b());
            wonders.push_back(Olympia_b());
            wonders.push_back(Rhodos_b());
            wonders.push_back(Ephesos_b());
            wonders.push_back(Alexandria_b());
            wonders.push_back(Halikarnassos_b());
        }

        void CreateDecks(){
            std::vector<DMAG::Card> cards;

            // Raw Material
            cards.push_back(Card(CARD_ID::lumber_yard, "Lumber Yard", CARD_TYPE::materials, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 2, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::stone_pit, "Stone Pit", CARD_TYPE::materials, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::clay_pool, "Clay Pool", CARD_TYPE::materials, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::ore_vein, "Ore Vein", CARD_TYPE::materials, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 2, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::tree_farm, "Tree Farm", CARD_TYPE::materials, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 1}, {0, 0, 0, 1, 1}));
            cards.push_back(Card(CARD_ID::excavation, "Excavation", CARD_TYPE::materials, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 1}, {0, 1, 1, 1, 1}));
            cards.push_back(Card(CARD_ID::clay_pit, "Clay Pit", CARD_TYPE::materials, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 1}, {1, 1, 1, 1, 1}));
            cards.push_back(Card(CARD_ID::timber_yard, "Timber Yard", CARD_TYPE::materials, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 1}, {1, 1, 1, 1, 1}));
            cards.push_back(Card(CARD_ID::forest_cave, "Forest Cave", CARD_TYPE::materials, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 1}, {0, 0, 1, 1, 1}));
            cards.push_back(Card(CARD_ID::mine, "Mine", CARD_TYPE::materials, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 1}, {0, 0, 0, 1, 1}));
            cards.push_back(Card(CARD_ID::sawmill, "Sawmill", CARD_TYPE::materials, 2, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 1}, {1, 2, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::quarry, "Quarry", CARD_TYPE::materials, 2, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 1}, {1, 2, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::brickyard, "Brickyard", CARD_TYPE::materials, 2, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 1}, {1, 2, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::foundry, "Foundry", CARD_TYPE::materials, 2, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 1}, {1, 2, 2, 2, 2}));
            // Manufactured Good
            cards.push_back(Card(CARD_ID::loom, "Loom", CARD_TYPE::manufactured, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 1, 2, 2}));
            cards.push_back(Card(CARD_ID::glassworks, "Glassworks", CARD_TYPE::manufactured, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 1, 2, 2}));
            cards.push_back(Card(CARD_ID::press, "Press", CARD_TYPE::manufactured, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 1, 2, 2}));
            cards.push_back(Card(CARD_ID::loom, "Loom", CARD_TYPE::manufactured, 2, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::glassworks, "Glassworks", CARD_TYPE::manufactured, 2, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::press, "Press", CARD_TYPE::manufactured, 2, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 2, 2, 2}));
            // Civilian Structure
            cards.push_back(Card(CARD_ID::altar, "Altar", CARD_TYPE::civilian, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::theater, "Theater", CARD_TYPE::civilian, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 1, 2, 2}));
            cards.push_back(Card(CARD_ID::pawnshop, "Pawnshop", CARD_TYPE::civilian, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 1, 1, 2}));
            cards.push_back(Card(CARD_ID::baths, "Baths", CARD_TYPE::civilian, 1, CARD_ID::none, {0, 0, 0, 1, 0, 0, 0, 0}, {1, 1, 1, 1, 2}));
            cards.push_back(Card(CARD_ID::temple, "Temple", CARD_TYPE::civilian, 2, CARD_ID::altar, {1, 0, 1, 0, 0, 1, 0, 0}, {1, 1, 1, 2, 2}));
            cards.push_back(Card(CARD_ID::courthouse, "Courthouse", CARD_TYPE::civilian, 2, CARD_ID::scriptorium, {0, 0, 2, 0, 1, 0, 0, 0}, {1, 1, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::statue, "Statue", CARD_TYPE::civilian, 2, CARD_ID::theater, {1, 2, 0, 0, 0, 0, 0, 0}, {1, 1, 1, 1, 2}));
            cards.push_back(Card(CARD_ID::aqueduct, "Aqueduct", CARD_TYPE::civilian, 2, CARD_ID::baths, {0, 0, 3, 0, 0, 0, 0, 0}, {1, 1, 1, 1, 2}));
            cards.push_back(Card(CARD_ID::gardens, "Gardens", CARD_TYPE::civilian, 3, CARD_ID::statue, {1, 0, 2, 0, 0, 0, 0, 0}, {1, 2, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::town_hall, "Town Hall", CARD_TYPE::civilian, 3, CARD_ID::none, {0, 1, 0, 2, 0, 0, 1, 0}, {1, 1, 2, 3, 3}));
            cards.push_back(Card(CARD_ID::senate, "Senate", CARD_TYPE::civilian, 3, CARD_ID::library, {2, 1, 0, 1, 0, 0, 0, 0}, {1, 1, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::pantheon, "Pantheon", CARD_TYPE::civilian, 3, CARD_ID::temple, {0, 1, 2, 0, 1, 1, 1, 0}, {1, 1, 1, 2, 2}));
            cards.push_back(Card(CARD_ID::palace, "Palace", CARD_TYPE::civilian, 3, CARD_ID::none, {1, 1, 1, 1, 1, 1, 1, 0}, {1, 1, 1, 1, 2}));
            // Commercial Structure
            cards.push_back(Card(CARD_ID::tavern, "Tavern", CARD_TYPE::commercial, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 2, 2, 3}));
            cards.push_back(Card(CARD_ID::east_trading_post, "East Trading Post", CARD_TYPE::commercial, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 1, 1, 2}));
            cards.push_back(Card(CARD_ID::west_trading_post, "West Trading Post", CARD_TYPE::commercial, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 1, 1, 2}));
            cards.push_back(Card(CARD_ID::marketplace, "Marketplace", CARD_TYPE::commercial, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 1, 2, 2}));
            cards.push_back(Card(CARD_ID::forum, "Forum", CARD_TYPE::commercial, 2, CARD_ID::east_trading_post, {0, 0, 2, 0, 0, 0, 0, 0}, {1, 1, 1, 2, 3}));
            cards.push_back(Card(CARD_ID::caravansery, "Caravansery", CARD_TYPE::commercial, 2, CARD_ID::marketplace, {2, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 2, 3, 3}));
            cards.push_back(Card(CARD_ID::vineyard, "Vineyard", CARD_TYPE::commercial, 2, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 1, 2, 2}));
            cards.push_back(Card(CARD_ID::bazar, "Bazar", CARD_TYPE::commercial, 2, CARD_ID::none, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 1, 1, 2}));
            cards.push_back(Card(CARD_ID::haven, "Haven", CARD_TYPE::commercial, 3, CARD_ID::forum, {1, 1, 0, 0, 1, 0, 0, 0}, {1, 2, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::lighthouse, "Lighthouse", CARD_TYPE::commercial, 3, CARD_ID::caravansery, {0, 0, 0, 1, 0, 1, 0, 0}, {1, 1, 1, 2, 2}));
            cards.push_back(Card(CARD_ID::chamber_of_commerce, "Chamber of Commerce", CARD_TYPE::commercial, 3, CARD_ID::none, {0, 0, 2, 0, 0, 0, 1, 0}, {0, 1, 1, 2, 2}));
            cards.push_back(Card(CARD_ID::arena, "Arena", CARD_TYPE::commercial, 3, CARD_ID::dispensary, {0, 1, 0, 2, 0, 0, 0, 0}, {1, 1, 2, 2, 3}));
            // Military Structure
            cards.push_back(Card(CARD_ID::stockade, "Stockade", CARD_TYPE::military, 1, CARD_ID::none, {1, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 1, 1, 2}));
            cards.push_back(Card(CARD_ID::barracks, "Barracks", CARD_TYPE::military, 1, CARD_ID::none, {0, 1, 0, 0, 0, 0, 0, 0}, {1, 1, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::guard_tower, "Guard Tower", CARD_TYPE::military, 1, CARD_ID::none, {0, 0, 1, 0, 0, 0, 0, 0}, {1, 2, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::walls, "Walls", CARD_TYPE::military, 2, CARD_ID::none, {0, 0, 0, 3, 0, 0, 0, 0}, {1, 1, 1, 1, 2}));
            cards.push_back(Card(CARD_ID::training_ground, "Training Ground", CARD_TYPE::military, 2, CARD_ID::none, {1, 2, 0, 0, 0, 0, 0, 0}, {0, 1, 1, 2, 3}));
            cards.push_back(Card(CARD_ID::stables, "Stables", CARD_TYPE::military, 2, CARD_ID::apothecary, {1, 1, 1, 0, 0, 0, 0, 0}, {1, 1, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::archery_range, "Archery Range", CARD_TYPE::military, 2, CARD_ID::workshop, {2, 1, 0, 0, 0, 0, 0, 0}, {1, 1, 1, 2, 2}));
            cards.push_back(Card(CARD_ID::fortifications, "Fortifications", CARD_TYPE::military, 3, CARD_ID::walls, {0, 3, 0, 1, 0, 0, 0, 0}, {1, 1, 1, 1, 2}));
            cards.push_back(Card(CARD_ID::circus, "Circus", CARD_TYPE::military, 3, CARD_ID::training_ground, {0, 1, 0, 3, 0, 0, 0, 0}, {0, 1, 2, 3, 3}));
            cards.push_back(Card(CARD_ID::arsenal, "Arsenal", CARD_TYPE::military, 3, CARD_ID::none, {2, 1, 0, 0, 1, 0, 0, 0}, {1, 1, 1, 1, 2}));
            cards.push_back(Card(CARD_ID::siege_workshop, "Siege Workshop", CARD_TYPE::military, 3, CARD_ID::laboratory, {1, 0, 3, 0, 0, 0, 0, 0}, {1, 1, 2, 2, 2}));
            // Scientific Structure
            cards.push_back(Card(CARD_ID::apothecary, "Apothecary", CARD_TYPE::scientific, 1, CARD_ID::none, {0, 0, 0, 0, 1, 0, 0, 0}, {1, 1, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::workshop, "Workshop", CARD_TYPE::scientific, 1, CARD_ID::none, {0, 0, 0, 0, 0, 1, 0, 0}, {1, 1, 1, 1, 2}));
            cards.push_back(Card(CARD_ID::scriptorium, "Scriptorium", CARD_TYPE::scientific, 1, CARD_ID::none, {0, 0, 0, 0, 0, 0, 1, 0}, {1, 2, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::dispensary, "Dispensary", CARD_TYPE::scientific, 2, CARD_ID::apothecary, {0, 2, 0, 0, 0, 1, 0, 0}, {1, 2, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::laboratory, "Laboratory", CARD_TYPE::scientific, 2, CARD_ID::workshop, {0, 0, 2, 0, 0, 0, 1, 0}, {1, 1, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::library, "Library", CARD_TYPE::scientific, 2, CARD_ID::scriptorium, {0, 0, 0, 2, 1, 0, 0, 0}, {1, 1, 1, 2, 2}));
            cards.push_back(Card(CARD_ID::school, "School", CARD_TYPE::scientific, 2, CARD_ID::none, {1, 0, 0, 0, 0, 0, 1, 0}, {1, 1, 1, 1, 2}));
            cards.push_back(Card(CARD_ID::lodge, "Lodge", CARD_TYPE::scientific, 3, CARD_ID::dispensary, {0, 0, 2, 0, 1, 0, 1, 0}, {1, 1, 1, 2, 2}));
            cards.push_back(Card(CARD_ID::observatory, "Observatory", CARD_TYPE::scientific, 3, CARD_ID::laboratory, {0, 2, 0, 0, 1, 1, 0, 0}, {1, 1, 1, 1, 2}));
            cards.push_back(Card(CARD_ID::university, "University", CARD_TYPE::scientific, 3, CARD_ID::library, {2, 0, 0, 0, 0, 1, 1, 0}, {1, 1, 2, 2, 2}));
            cards.push_back(Card(CARD_ID::academy, "Academy", CARD_TYPE::scientific, 3, CARD_ID::school, {0, 0, 0, 3, 0, 1, 0, 0}, {1, 1, 1, 1, 2}));
            cards.push_back(Card(CARD_ID::study, "Study", CARD_TYPE::scientific, 3, CARD_ID::school, {1, 0, 0, 0, 1, 0, 1, 0}, {1, 1, 2, 2, 2}));
            // Guild
            std::vector<DMAG::Card> guild_cards;
            guild_cards.push_back(Card(CARD_ID::workers, "Workers Guild", CARD_TYPE::guild, 3, CARD_ID::none, {1, 2, 1, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 0}));
            guild_cards.push_back(Card(CARD_ID::craftsmens, "Craftsmens Guild", CARD_TYPE::guild, 3, CARD_ID::none, {0, 2, 0, 2, 0, 0, 0, 0}, {0, 0, 0, 0, 0}));
            guild_cards.push_back(Card(CARD_ID::traders, "Traders Guild", CARD_TYPE::guild, 3, CARD_ID::none, {0, 0, 0, 0, 1, 1, 1, 0}, {0, 0, 0, 0, 0}));
            guild_cards.push_back(Card(CARD_ID::philosophers, "Philosophers Guild", CARD_TYPE::guild, 3, CARD_ID::none, {0, 0, 3, 0, 1, 0, 1, 0}, {0, 0, 0, 0, 0}));
            guild_cards.push_back(Card(CARD_ID::spies, "Spies Guild", CARD_TYPE::guild, 3, CARD_ID::none, {0, 0, 3, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}));
            guild_cards.push_back(Card(CARD_ID::magistrates, "Magistrates Guild", CARD_TYPE::guild, 3, CARD_ID::none, {3, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 0, 0}));
            guild_cards.push_back(Card(CARD_ID::shipowners, "Shipowners Guild", CARD_TYPE::guild, 3, CARD_ID::none, {3, 0, 0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 0}));
            guild_cards.push_back(Card(CARD_ID::strategists, "Strategists Guild", CARD_TYPE::guild, 3, CARD_ID::none, {0, 2, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 0, 0}));
            guild_cards.push_back(Card(CARD_ID::scientists, "Scientists Guild", CARD_TYPE::guild, 3, CARD_ID::none, {2, 2, 0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0}));
            guild_cards.push_back(Card(CARD_ID::builders, "Builders Guild", CARD_TYPE::guild, 3, CARD_ID::none, {0, 0, 2, 2, 0, 1, 0, 0}, {0, 0, 0, 0, 0}));

            // insert cards to decks
            for (DMAG::Card const& card : cards) {
                int amount = card.GetAmount(this->number_of_players);
                for (int i = 0; i < amount; i++) {
                    deck[card.GetEra()-1].push_back(card);
                }
            }

            // add guild cards to deck (5 cards to 3 players, 6 cards to 4 players, ...)
            srand(time(0));
            random_shuffle(guild_cards.begin(), guild_cards.end());
            for (int i = 0; i < this->number_of_players + 2; i++) {
                deck[2].push_back(guild_cards[i]);
            }
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
            fp.Init(NUM_PLAYERS);

        }

        void Close(){

            // deallocate memory
        }

        void ShowInfo(){
            /*
             * Will print to the stdout all the information formatted
             * as JSON object.
             * This will only be called if the variable NDEBUG is not
             * set. CMAKE sets that when compiling as RELEASE
             */

        }

        void Loop(){
            json json_object;
            std::string command, argument;
            while(InGame()){
                while(!fp.ArePlayersReady());
                for(int i = 0; i < number_of_players; i++){
                    json_object = fp.ReadMessages(i);
                    // handle command inside json_object
                    command = json_object["command"]["subcommand"];
                    argument = json_object["command"]["argument"];
                    if(command == "build_structure"){
                        Card selected = GetCardByName(argument);
                        player_list[i]->BuildStructure(selected, player_list[i]->GetHandCards(), player_list[i]->CanPlayFree(selected)); // find card before calling this
                    }else if(command == "build_guild"){
                        //do something
                    }else if(command == "build_wonder"){
                        Card sacrifice = GetCardByName(argument);
                        player_list[i]->BuildWonder(sacrifice);
                    }else if(command == "discard"){
                        Card discard = GetCardByName(argument);
                        player_list[i]->Discard(); //Gives player 3 coins.
                        discard_pile.push_back(discard);
                    }

                    // Moves the game to the next turn.
                    NextTurn(0); // Have to fix this, and the method.

                    // TODO: show info
#ifndef NDEBUG
                    ShowInfo();
#endif


                    //get commands

                    //calculate stuff

                    //end game?
                }
            }

        }
};


        int main()
        {
            Game g;

            g.Init();
            g.NewGame(NUM_PLAYERS);

            //    g.NextTurn(p, 0); //this function is not completed
            g.Loop();
            g.Close();

            return 0;
        }

