#include <iostream>
#include <card.h>
#include <filer.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <resources.h>
#include <vector>
#include <player.h>

using json = nlohmann::json;

Filer::Filer(){
	turn = 1;
}

int Filer::Init(int player_count){
	this->player_count = player_count;
	return 0;
}


/*
 * fclose returns 0 if no error
 * so setting retval to return value
 * and returning that for debugging purposes
 */

int Filer::WriteMessage(json message, std::string path){
	// out_file.open("./io/game_status.json");
	out_file.open(path);
	out_file  << message << std::endl;
	out_file.close();
	return 0;
}

json Filer::ReadMessages(int player_number){
	json ret;
	if(player_number >= player_count)
		return json({});
	char filename[30];
	sprintf(filename, "./io/player_%d.json", player_number+1);
	player_input.open(filename);
	player_input >> ret;

	player_input.close(); //Puts the cursor back at the start
	return ret;
}

bool Filer::ArePlayersReady(){
	players_ready.open("./io/ready.txt");
	std::string ready;
	for(int i = 0; i < player_count; i++){
		getline(players_ready, ready);
		if(ready != "ready"){
			//std::cout << i << " is not ready" <<std::endl;
			players_ready.close();
			return false;
		}
	}

	players_ready.close();
	std::ofstream clear_ready("./io/ready.txt", std::ofstream::out | std::ofstream::trunc);
	clear_ready.close();
	return true;
}

void Filer::StartLog(int log_id){
	char file[64];
	sprintf(file, "./logs/7w_game_%d.csv", log_id);
	log_file_path = file;
	
	log_file.open(log_file_path, std::ofstream::out);
	
	log_file << "era" << ",";
	log_file << "turn" << ",";
	log_file << "player" << ",";
	for (int i = 0; i < 7; i++) {
		log_file << "hand card " << i << ",";
	}
	log_file << "action" << ",";
	log_file << "card played" << "\n";

	log_file.close();
}

void Filer::WriteLog(int era, int turn, int player_id, std::vector<DMAG::Card> hand_cards, std::string action, std::string card){
	log_file.open(log_file_path, std::ofstream::app);

	log_file << era << ",";
	log_file << turn << ",";
	log_file << player_id << ",";
	for (int i = 0; i < 7; i++) {
		if (i < hand_cards.size())
			log_file << hand_cards[i].GetName();
		log_file << ",";
	}
	log_file << action << ",";
	log_file << card << "\n";

	log_file.close();
}

void Filer::WriteMatchLog(std::vector<DMAG::Player*> player_list, int log_id) {
	char file[64];
	sprintf(file, "../logs/7w_match_%d.csv", log_id);
	log_file_path = file;

	log_file.open(log_file_path, std::ofstream::out);

	log_file << " " << ",";
	log_file << "player_1" << ",";
	log_file << "player_2" << ",";
	log_file << "player_3" << "\n";

	int number_of_players = 3;
	log_file << "Victory Points";
	// Victory points row
	for(int i = 0; i < number_of_players; i++) {
		log_file << "," << player_list[i]->CalculateScore();
	}
	log_file << "\n";

	// Civilian points
	log_file << "Civilian Points";
	for(int i = 0; i < number_of_players; i++) {
		log_file << "," << player_list[i]->CalculateCivilianScore();
	}
	log_file << "\n";

	// Commercial points
	log_file << "Commercial Points";
	for(int i = 0; i < number_of_players; i++) {
		log_file << "," << player_list[i]->CalculateCommercialScore();
	}
	log_file << "\n";

	// Guild points
	log_file << "Guild Points";
	for(int i = 0; i < number_of_players; i++) {
		log_file << "," << player_list[i]->CalculateGuildScore();
	}
	log_file << "\n";

	// Military points
	log_file << "Military Points";
	for(int i = 0; i < number_of_players; i++) {
		log_file << "," << player_list[i]->CalculateMilitaryScore();
	}
	log_file << "\n";

	// scientific
	log_file << "Scientific Points";
	for(int i = 0; i < number_of_players; i++) {
		log_file << "," << player_list[i]->CalculateScientificScore();
	}
	log_file << "\n";

	// wonder 
	log_file << "Wonder Points"; 
	for(int i = 0; i < number_of_players; i++) {
		log_file << "," << player_list[i]->CalculateWonderScore();
	}
	log_file << "\n";


	// resources
	std::map<int, int> rs_player_1 = player_list[0]->GetResources();
	std::map<int, int> rs_player_2 = player_list[1]->GetResources();
	std::map<int, int> rs_player_3 = player_list[2]->GetResources();

	// wood
	log_file << "Wood" << "," << rs_player_1[RESOURCE::wood] << "," << rs_player_2[RESOURCE::wood] << "," << rs_player_3[RESOURCE::wood] << "\n";

	// ore
	log_file << "Ore" << "," << rs_player_1[RESOURCE::ore] << "," << rs_player_2[RESOURCE::ore] << "," << rs_player_3[RESOURCE::ore] << "\n";

	// clay 
	log_file << "Clay" << "," << rs_player_1[RESOURCE::clay] << "," << rs_player_2[RESOURCE::clay] << "," << rs_player_3[RESOURCE::clay] << "\n";

	// stone
	log_file << "Stone" << "," << rs_player_1[RESOURCE::stone] << "," << rs_player_2[RESOURCE::stone] << "," << rs_player_3[RESOURCE::stone] << "\n";

	// loom 
	log_file << "Loom" << "," << rs_player_1[RESOURCE::loom] << "," << rs_player_2[RESOURCE::loom] << "," << rs_player_3[RESOURCE::loom] << "\n";

	// glass
	log_file << "Glass" << "," << rs_player_1[RESOURCE::glass] << "," << rs_player_2[RESOURCE::glass] << "," << rs_player_3[RESOURCE::glass] << "\n";

	// papyrus
	log_file << "Papyrus" << "," << rs_player_1[RESOURCE::papyrus] << "," << rs_player_2[RESOURCE::papyrus] << "," << rs_player_3[RESOURCE::papyrus] << "\n";

	// gear
	log_file << "Gear" << "," << rs_player_1[RESOURCE::gear] << "," << rs_player_2[RESOURCE::gear] << "," << rs_player_3[RESOURCE::gear] << "\n";

	// compass
	log_file << "Compass" << "," << rs_player_1[RESOURCE::compass] << "," << rs_player_2[RESOURCE::compass] << "," << rs_player_3[RESOURCE::compass] << "\n";

	// tablet 
	log_file << "Tablet" << "," << rs_player_1[RESOURCE::tablet] << "," << rs_player_2[RESOURCE::tablet] << "," << rs_player_3[RESOURCE::tablet] << "\n";

	// coins
	log_file << "Coins" << "," << rs_player_1[RESOURCE::coins] << "," << rs_player_2[RESOURCE::coins] << "," << rs_player_3[RESOURCE::coins] << "\n";

	// shields
	log_file << "Shields" << "," << rs_player_1[RESOURCE::shields] << "," << rs_player_2[RESOURCE::shields] << "," << rs_player_3[RESOURCE::shields] << "\n";

	log_file.close();
}
