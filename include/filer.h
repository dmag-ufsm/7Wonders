#include <fstream>
#include <iostream>

class Filer{
	private:
		std::string output;
		int turn;
		std::ofstream out_file;
		std::ofstream turn_file;
		std::ifstream player_input[7];
		int player_count;

	public:
		Filer();
		int init(int player_count);
		int close(void);
		int build_message(std::string s);
		int write_message();
		std::string read_messages(int player_number);
};
