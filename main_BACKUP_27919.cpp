#include <iostream>
#include <card.h>
#include <player.h>
#include <deck.h>
#include <amqp.h> // The rabbit-mq broker library
#include <list>
<<<<<<< Updated upstream
=======
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <filer.h>
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>

#define NUM_PLAYERS 3

using json = nlohmann::json;
>>>>>>> Stashed changes

using namespace std;
using namespace DMAG;

<<<<<<< Updated upstream
// define global variables
Deck d;
list<Player> player_list;
=======
class Game{
    private:
        vector<Player*> player_list;
        unsigned char number_of_players;
        short era;
        unsigned char turn;
        vector<Wonder*> wonders;
        vector<Card> deck[3]; // need to change to Deck deck[3];
        vector<Card> discard_pile; // need to change to Deck discard_pile;

        Filer fp;

    public:
        bool previously_game_played;

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

        Wonder* GetWonderByName(std::string name){
            for (auto w: wonders) {
                if (w->GetName().compare(name) == 0)
                    return w;
            }
            exit(1); //You typed a wonder that does not exist
        }

        int GetResourceByName(std::string name){
            if (name == "Wood") return RESOURCE::wood;
            if (name == "Ore") return RESOURCE::ore;
            if (name == "Clay") return RESOURCE::clay;
            if (name == "Stone") return RESOURCE::stone;
            if (name == "Loom") return RESOURCE::loom;
            if (name == "Glass") return RESOURCE::glass;
            if (name == "Papyrus") return RESOURCE::papyrus;
            if (name == "Gear") return RESOURCE::gear;
            if (name == "Compass") return RESOURCE::compass;
            if (name == "Tablet") return RESOURCE::tablet;
            return -1;
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

        void NextTurn(vector<string> log){

            // TODO: check if the player has the wonder effects that make
            // possible to play another card in the same round and do it

            turn++;

            // if end of an era (turns 7 14 and 21)
            if (turn % 7 == 0) {
                for (Player* & player : player_list) {
                    player->FreeCardOnce(true);
                    player->Battle(era);
                }

                era++;
                cout << "-----------------------------------------------------------------\n";
                if(era < 4)
                    cout << "Nova era: "<< era << endl;
                else
                    cout << "End of game" << endl;
                // transfer the remaining card in each player's hand to the discarded card list
                for (Player* & player : player_list) {
                    vector<Card> cards = player->GetHandCards();
                    // cards must be size 1!!
                    for (int i = 0; i < cards.size(); i++)
                        discard_pile.push_back(cards[i]);
                }

                GiveCards(log);
            }
            else {
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
                    neighbor = clockwise ? player->GetWestNeighbor() : player->GetEastNeighbor();
                    neighbor_deck = neighbor->GetHandCards();
                    neighbor->ReceiveCards(player_deck);
                    player = neighbor;
                    player_deck = neighbor_deck;
                    //cout << "Passou" << endl;
                } while (p1 != player);
                //cout << "Passou tudo" << endl;
            }
        }


        /*
          If using defaults, call as GiveWonders(null, 0);
          Args:
          card_names: the ordered list of cards that each player should get
        */
        void GiveCards(std::vector<std::string> card_names){
            vector<Card> cards;
            int card_idx = 0;

            random_shuffle(deck[era-1].begin(), deck[era-1].end());

            if(!previously_game_played){
                for (Player* & player : player_list) {
                    cards.clear();
                    for (int i = 0; i < 7; i++) {
                        cards.push_back(deck[era-1][card_idx++]);
                    }
                    player->ReceiveCards(cards);
                }
            }
            else{
              for (Player* & player : player_list) {
                  cards.clear();
                  for (int i = 0; i < 7; i++) {
                      cards.push_back(GetCardByName(card_names.front()));
                      card_names.erase(card_names.begin());
                  }
                  player->ReceiveCards(cards);
              }
            }
        }


        void GiveWonders(std::vector<std::string> log){
            bool wonder_availability[7];
            for (int i = 0; i < 7; i++)
                wonder_availability[i] = true;

            if (!previously_game_played) {
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
            } else {
                for (Player* & player : player_list) {
                    player->SetWonder(GetWonderByName(log.front()));
                    log.erase(log.begin());
                }
            }
        }

        void CreateWonders(){
            wonders.push_back(new Gizah_a());
            wonders.push_back(new Babylon_a());
            wonders.push_back(new Olympia_a());
            wonders.push_back(new Rhodos_a());
            wonders.push_back(new Ephesos_a());
            wonders.push_back(new Alexandria_a());
            wonders.push_back(new Halikarnassos_a());
            wonders.push_back(new Gizah_b());
            wonders.push_back(new Babylon_b());
            wonders.push_back(new Olympia_b());
            wonders.push_back(new Rhodos_b());
            wonders.push_back(new Ephesos_b());
            wonders.push_back(new Alexandria_b());
            wonders.push_back(new Halikarnassos_b());
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
            // obs.: Forum can be constructed for free if the player has either the East Trading Post or the West Trading Post.
            // Therefore, freeWithId will be -1 and will be treated on player.cpp, therefore avoiding the need for vectors.
            // It's not the prettiest, but it's a simple and efficient solution, as Forum is the only card that carries this exception.
            cards.push_back(Card(CARD_ID::forum, "Forum", CARD_TYPE::commercial, 2, -1, {0, 0, 2, 0, 0, 0, 0, 0}, {1, 1, 1, 2, 3}));
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
            // if the game is loaded, add all guild cards to 3rd era deck
            for (int i = 0; i < previously_game_played ? guild_cards.size() : this->number_of_players + 2; i++) {
                deck[2].push_back(guild_cards[i]);
            }
        }


        int NewGame(int _players, vector<string> log){
          // same as Init. if _players is zero, will use default settings
          if(_players == 0)
            _players = NUM_PLAYERS;

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

            CreateDecks();
            GiveWonders(log);
            GiveCards(log);

            return 0;
        }

        // _num_players should be 0 if starting a normal simulation game
        // If game reads config from file, _num_players should be the number
        // Of players stated in the file
        void Init(int _num_players){
          if(_num_players == 0){
            fp.Init(NUM_PLAYERS);
          }
          else{
            fp.Init(_num_players);
          }
          CreateWonders();
          return;

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

        void WriteGameStatus() {
            json status;
            std::vector<DMAG::Card> cards;
            std::vector<std::string> card_names;
            std::map<int, int> resources;
            DMAG::Wonder* wonder;

            status["game"]["era"] = era;
            status["game"]["turn"] = turn;

            for (int i = 0; i < number_of_players; i++) {
                wonder = player_list[i]->GetBoard();
                status["players"][to_string(i)]["wonder_id"] = wonder->GetId();
                status["players"][to_string(i)]["wonder_name"] = wonder->GetName();
                status["players"][to_string(i)]["wonder_stage"] = wonder->GetStage();

                cards = player_list[i]->GetHandCards();
                card_names.clear();
                for (DMAG::Card c : cards)
                    card_names.push_back(c.GetName());
                status["players"][to_string(i)]["cards_hand"] = card_names;

                cards = player_list[i]->GetPlayedCards();
                card_names.clear();
                for (DMAG::Card c : cards)
                    card_names.push_back(c.GetName());
                status["players"][to_string(i)]["cards_played"] = card_names;

                cards = player_list[i]->GetPlayableCards();
                card_names.clear();
                for (DMAG::Card c : cards)
                    card_names.push_back(c.GetName());
                status["players"][to_string(i)]["cards_playable"] = card_names;

                resources = player_list[i]->GetResources();
                status["players"][to_string(i)]["resources"]["wood"] = resources[RESOURCE::wood];
                status["players"][to_string(i)]["resources"]["ore"] = resources[RESOURCE::ore];
                status["players"][to_string(i)]["resources"]["clay"] = resources[RESOURCE::clay];
                status["players"][to_string(i)]["resources"]["stone"] = resources[RESOURCE::stone];
                status["players"][to_string(i)]["resources"]["loom"] = resources[RESOURCE::loom];
                status["players"][to_string(i)]["resources"]["glass"] = resources[RESOURCE::glass];
                status["players"][to_string(i)]["resources"]["papyrus"] = resources[RESOURCE::papyrus];
                status["players"][to_string(i)]["resources"]["gear"] = resources[RESOURCE::gear];
                status["players"][to_string(i)]["resources"]["compass"] = resources[RESOURCE::compass];
                status["players"][to_string(i)]["resources"]["tablet"] = resources[RESOURCE::tablet];
                status["players"][to_string(i)]["resources"]["coins"] = resources[RESOURCE::coins];
                status["players"][to_string(i)]["resources"]["shields"] = resources[RESOURCE::shields];
            }

            fp.WriteMessage(status);
        }

        void TryBuildingOrDiscard(Player* p, Card selected){
            if(p->BuildStructure(selected, p->GetHandCards(), false)) {
                cout << "<BuildStructure OK>" << endl;
            } else {
                cout << "<BuildStructure NOK>" << endl;
                cout << "<Discarding Card Instead>" << endl;
                p->Discard(selected);
                discard_pile.push_back(selected);
            }
        }

        void Loop(vector<string> log){
            json json_object;
            std::string command, argument, extra;
            while(InGame()){

                WriteGameStatus();

                cout << "\n:::TURN " << (short)this->turn << ":::" << endl;

                // Print Playable Cards for each player.
                for (int i = 0; i < number_of_players; i++) {
                    cout << "> Playable cards for player " << i << ":" << endl;
                    for (DMAG::Card const& card : player_list[i]->GetPlayableCards()) {
                        cout << "  (" << i << ") " << card.GetName() << endl;
                    }
                }

                cout << "<Waiting players...>" << endl;
                while(!fp.ArePlayersReady());

                for(int i = 0; i < number_of_players; i++){
                    json_object = fp.ReadMessages(i);
                    // handle command inside json_object
                    command = json_object["command"]["subcommand"];
                    argument = json_object["command"]["argument"];

                    // Only the player with Halikarnassos will have something written on extra.
                    extra = json_object["command"]["extra"];

                    // Turns 6 13 and 20 -> last card in hand card.
                    // If the player doesn't have the ability to play the seventh card,
                    // just skip the turn (i.e. don't do anything).
                    if (this->turn == 6 || this->turn == 13 || this->turn == 20) {
                        if (!player_list[i]->PlaySeventh()) continue;
                    }

                    if (command == "build_structure"){
                        Card selected = GetCardByName(argument);
                        TryBuildingOrDiscard(player_list[i], selected);

                    } else if (command == "build_hand_free"){
                        Card selected = GetCardByName(argument);
                        if (player_list[i]->BuildHandFree(selected))
                            cout << "BuildHandFree OK>" << endl;
                        else
                            TryBuildingOrDiscard(player_list[i], selected);

                    } else if(command == "build_wonder"){
                        Card sacrifice = GetCardByName(argument);
                        if (player_list[i]->BuildWonder(sacrifice))
                            cout << "<BuildWonder OK>" << endl;
                        else
                            cout << "<BuildWonder NOK>" << endl;

                    } else if(command == "discard"){
                        Card discard = GetCardByName(argument);
                        player_list[i]->Discard(discard); //Gives player 3 coins.
                        cout << "<Discard OK>" << endl;
                        discard_pile.push_back(discard);
                    }
                }

                // Moves the game to the next turn.
                // VERY IMPORTANT: call player->ResetUsed() for each player at the end of a turn!
                for (int i = 0; i < player_list.size(); i++) {
                    player_list[i]->ResetUsed();
                    // If extra is not empty, try to build a card from the discard pile for free.
                    // (will only work if the player has the required stage for Halikarnassos)
                    if (!extra.empty()) {
                        Card c = GetCardByName(extra);
                        if (player_list[i]->BuildDiscardFree(c, discard_pile)) {
                            cout << "<BuildDiscardFree OK>" << endl;
                        }
                    }
                }

                NextTurn(log);


                // TODO: show info
#ifndef NDEBUG
                ShowInfo();
#endif


                //get commands
>>>>>>> Stashed changes

// Initiate global variables

void game_init(int _players){
    d = Deck();
    for(int i = 0; i<_players; i++){
        player_list.push_back(Player());
    }
    return;
}

void game_loop(){

    // create connection with RabbitMQ

    //show info

    //get commands

<<<<<<< Updated upstream
    //calculate stuff

    //end game?
}

int main()
{
    game_init(3);
    game_loop();
    return 0;
}


=======
        /*
         * Args will be used to tell how to load
         * a new game.
         * We'll use -f filename to tell a file that
         * has information about a previously played game
         * (or just a specific card configuration)
         */
        int main(int argc, char **argv)
        {
            Game g;
            std::vector<std::string> log;
            int _number_of_players = NUM_PLAYERS;

            std::ifstream file;
            if(argc > 1){
                file.open(argv[1]);
                string line;
                getline(file, line);
                _number_of_players = std::stoi(line);

                while(getline(file, line)){
                  log.push_back(line);
                }

                g.previously_game_played = true;
            }
            else{
                g.previously_game_played = false;
            }

            g.Init(_number_of_players);
            g.NewGame(_number_of_players, log);

            //    g.NextTurn(p, 0); //this function is not completed
            g.Loop(log);
            g.Close();

            return 0;
        }
>>>>>>> Stashed changes
