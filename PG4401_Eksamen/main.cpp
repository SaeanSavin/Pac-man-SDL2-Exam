#include <iostream>
#include <filesystem>

#include "GameManager.h"
#include "SDL_Manager.h"

int main(int argc, char *argv[]) {

	std::cout << "Welcome to pac-man\n" << std::endl;
	std::string path = "../maps";
	std::vector<std::string> mapList{};
	int index = 1;
	int mapSelection = 0;

	
	for (const auto &entry : std::filesystem::directory_iterator(path)) {
		
		if (entry.path().has_extension()) {
			std::cout << index << ") " << entry.path().stem().string() << std::endl;
			std::cout << "" << std::endl;
			mapList.emplace_back(entry.path().string());
			index++;
		}
	}

	std::cout << "Choose map: ";
	std::cin >> mapSelection;
	std::cout << mapSelection << std::endl;

	std::cout << "\n\n\n" << std::endl;

	std::string name = mapList[mapSelection - 1];

	auto gm = std::make_unique<GameManager>();
	gm->play(name);

	return 0;

}