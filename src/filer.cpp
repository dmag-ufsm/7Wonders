#include <filer.h>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Filer::Filer(){
	turn = 1;
}

int Filer::init(int player_count){
	out_file.open("game_output.txt");
	turn_file.open("game_turn.txt");

	this->player_count = player_count;
	for(int i = 0; i<player_count; ++i){
		char filename[30];
		sprintf(filename, "player_%d.txt", i+1);
		player_input[i].open(filename);
	}
	return 0;
}


/*
 * fclose returns 0 if no error
 * so setting retval to return value
 * and returning that for debugging purposes
 */
int Filer::close(){
	for(int i = 0; i<player_count; i++){
		player_input[i].close();
	}
	out_file.close();
	turn_file.close();

	return 0;
}

int Filer::write_message(){

	for(auto & s: output){
		out_file << s << std::endl;
	}
	turn_file << turn << std::endl;

	turn++;
	output.clear();
	return 0;
}

int Filer::build_message(std::string s){
	if(s.empty())
		return 1;
	output += s;
	return 0;
}

std::string Filer::read_messages(int player_number){

	if(player_number >= player_count)
		return "";

	std::string aux, s;
	while(getline(player_input[player_number], aux)){
		s += aux;
	}
	return s;
}
