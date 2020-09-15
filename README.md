# 7 Wonders

Open source implementation of the game 7 Wonders focused on performance for the use and training of bots for the game. This is capable of running hundreds of games per minute, using simple JSON files as input and output.

## Building

First, clone the repository. Create a build subdir on the project root, then run cmake on the project build subdir, and you'll end up with a `7Wonders` binary file.

	git clone https://github.com/dmag-ufsm/Game.git
	cd Game
	mkdir build && cd build
	cmake .. && make

## Running

The game must be run from the binary file in the build folder:

	./7Wonders

As an option, the GUI can be used to assist when there are real players in the game. From the project root, run

	python3 Game.py <num_bots>

where `num_bots` is the number of bots that will play the game. This was built with a focus on 3-player matches, and will take care of executing the binary file contained in the build folder and making it visually and easier for the user to read and write in the JSON files.
	
## How it works



### Using a bot



## Environment

The code was developed and tested using the following environment:

- Linux OS
- gcc 8.3.0
- cmake 3.13.4
- Python 3.7.3 (only for GUI)

## License

Source code is released under the MIT License. See  `LICENSE`  for details.

Image assets used in the GUI were obtained from the game's [official website](https://rprod.com/index.php?page=download-2).
