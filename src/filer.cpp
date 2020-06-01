#include <iostream>
#include <card.h>
#include <filer.h>
#include <fstream>
#include <nlohmann/json.hpp>

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

int Filer::WriteMessage(json message){
	out_file.open("./io/game_status.json");
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
