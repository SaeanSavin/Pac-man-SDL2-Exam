#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include <vector>

class GameManager {

private:

	//void setFramerate(const int FPS);

public:

	GameManager();

	int play();
	void loadMap(std::string map, std::vector<std::vector<char>> &mapVector);
	//void drawMap(std::vector<std::vector<char>> &mapVector);
};




#endif