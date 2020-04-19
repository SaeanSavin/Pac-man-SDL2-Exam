#include "PromptManager.h"

#include <iostream>

int PromptManager::validatePrompt(int min, int max) {
	
	int input = 0;

	do {
		std::cin >> input;

		if ((std::cin.fail()) || (input < min || input > max)) {
			std::cout << "Invalid Input! Please enter a valid input: ";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

	} while (input < min || input > max);
	

	return input;
}
