#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include <vector>

#include "Player.h"
#include "Ghost.h"
#include "Sound.h"

class GameManager {

public:
	GameManager();
	void play(std::string name);
	void loadMap(std::string map, std::vector<std::vector<char>> &mapVector);
	void setTotalPlayerScore(int);

private:
	void setFramerate(const int FPS);
	std::pair<int, int> getTarget(enum class TargetType, std::shared_ptr<Character> enemy);
	std::pair<int, int> getTarget(TargetType mode, std::shared_ptr<Character> enemy, std::shared_ptr<Ghost> ghost);
	std::shared_ptr<Ghost> makeGhost(std::shared_ptr<Texture_Manager> texture_manager, SDL_Renderer *renderer, std::vector<SDL_Rect> &walkable, enum class GhostType type);
	int totalScore = 0;

};
#endif