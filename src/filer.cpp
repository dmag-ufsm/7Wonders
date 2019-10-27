#include <filer.h>

Filer::Filer(){
}

int Filer::init(int player_count){
	this->out_file = fopen("game_output.txt", "w");
	this->turn_file= fopen("game_turn.txt", "w");

	this->player_count = player_count;
	for(int i = 0; i<player_count; ++i){
		char filename[30];
		sprintf(filename, "player_%d.txt", i+1);
		this->player_input[i] = fopen(filename, "r");
	}
	return 0;
}


int Filer::close(){
	for(int i = 0; i<player_count; i++){
		fclose(player_input[i]);
	}
	return 0;

}

int Filer::write_message(){
	
	// do something

	this->output.clear();
	return 0;
}
