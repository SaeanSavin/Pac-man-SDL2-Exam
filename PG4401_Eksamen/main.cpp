#include <iostream>
#include <filesystem>

#include "GameManager.h"
#include "SDL_Manager.h"
#include "PromptManager.h"

int main(int argc, char *argv[]) {

	std::cout << "Welcome to pac-man\n" << std::endl;

	std::cout << "\nHow to play: \n"
		<< "Collect every pellets to advance to next round\n\n"
		<< "Keyboard:\n"
		<< "Up    =	 P1: W		P2: Arrow Up		P3: T		P4: I\n"
		<< "Down  =	 P1: S		P2: Arrow Down		P3: G		P4: K\n"
		<< "Left  =	 P1: A		P2: Arrow Left		P3: F		P4: J\n"
		<< "Right =	 P1: D		P2: Arrow Right		P3: H		P4: L\n\n"
		<< "Quit = ESC\n\n"
		<< "Gamepad (1 - 4 Players):\n"
		<< "Up = Dpad Up\n"
		<< "Down = Dpad Down\n"
		<< "Left = Dpad Left\n"
		<< "Right = Dpad Right\n" << std::endl;

	std::cout << "Press any key to continue..." << std::endl;
	std::cin.ignore();

	std::string path = "../maps";
	std::vector<std::string> mapList{};
	int index = 1;
	size_t mapSelection = 0;

	std::cout << "Select a map: " << std::endl;

	for (const auto &entry : std::filesystem::directory_iterator(path)) {
		
		if (entry.path().has_extension()) {
			std::cout << index << ") " << entry.path().stem().string() << std::endl;
			std::cout << "" << std::endl;
			mapList.emplace_back(entry.path().string());
			index++;
		}
	}

	std::cout << "Choose map: ";
	mapSelection = PromptManager::validatePrompt(1, index - 1);
	
	std::cout << "\n\n\n" << std::endl;

	std::string name = mapList[mapSelection - 1];

	auto gm = std::make_unique<GameManager>();
	gm->play(name);

	return 0;

}