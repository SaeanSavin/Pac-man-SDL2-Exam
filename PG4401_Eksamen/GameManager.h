#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include <vector>
#include "Player.h"
#include "Ghost.h"
#include "Sound.h"

class GameManager {

public:
	GameManager();
	int play(std::string name);
	void loadMap(std::string map, std::vector<std::vector<char>> &mapVector);

private:
	void setFramerate(const int FPS);
	std::pair<int, int> getTarget(enum TargetType, SDL_Rect *enemy);

};
#endif