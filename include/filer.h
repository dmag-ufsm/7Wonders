#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
class Filer{
	private:
		std::string output;
		int turn;
		std::ofstream out_file;
		std::ofstream turn_file;
		std::ifstream player_input;
		std::ifstream players_ready;
		int player_count;

	public:
		Filer();
		int Init(int player_count);
		int WriteMessage(json message);
		json ReadMessages(int player_number);
		bool ArePlayersReady();
};

/*

file format for loading previously played games
3                -> num players
halikarnassos_b  -> player 1 wonder
aldebaran_a      -> player 2 wonder
athenas_b        -> player 3 wonder

Loom						 -> 7 cards for player 1
Glassworks
Loom
Glassworks
Loom
Glassworks
Glassworks

Pedras					 -> 7 cards for player 2
clay
papiro
...

???? 						 -> 7 cards for player n

and continue for all eras


*/
