#include <cstdlib>
#include <iostream>

class Filer{
	private:
		std::string output;
		std::string turn;
		FILE *out_file;
		FILE *turn_file;
		FILE* player_input[7];
        int player_count;

	public:
		Filer();
		int init(int player_count);
		int close(void);
		int build_message(std::string s);
		int write_message();

};
