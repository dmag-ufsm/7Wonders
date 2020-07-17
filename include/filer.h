#include <fstream>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <player.h>

using json = nlohmann::json;
class Filer{
	private:
		std::string output;
		int turn;
		std::ofstream out_file;
		std::ofstream turn_file;
		std::ofstream log_file;
		std::ifstream player_input;
		std::ifstream players_ready;
		int player_count;
		std::string log_file_path;

	public:
		Filer();
		int Init(int player_count);
		int WriteMessage(json message, std::string path);
		json ReadMessages(int player_number);
		bool ArePlayersReady();

		void StartLog(int log_id);
		void WriteLog(int era, int turn, int player_id, std::vector<DMAG::Card> hand_cards, std::string action, std::string card);
		void WriteMatchLog(std::vector<DMAG::Player*> player_list, int log_id);
};
