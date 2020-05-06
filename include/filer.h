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
