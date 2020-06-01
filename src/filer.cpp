#include <iostream>
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
