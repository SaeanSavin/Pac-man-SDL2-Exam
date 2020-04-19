#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include <vector>

#include "Player.h"
#include "Ghost.h"

#include "SDL_Manager.h"

class GameManager {

public:
	GameManager();
	void play(std::string name);
	void loadMap(std::string map, std::vector<std::vector<char>> &mapVector);
	void setTotalPlayerScore(int);

private:
	void setFramerate(const int FPS);
	std::pair<int, int> getTarget(TargetType mode);
	std::pair<int, int> getTarget(enum class TargetType, std::shared_ptr<Player> enemy);
	std::pair<int, int> getTarget(enum class TargetType mode, std::shared_ptr<Player> enemy, std::shared_ptr<Ghost> ghost);
	std::pair<int, int> getTarget(TargetType mode, std::shared_ptr<Ghost> ghost);
	std::shared_ptr<Player> makePlayer(std::shared_ptr<Texture_Manager> texture_manager, std::shared_ptr<SDL_Manager> sdl_manager, SDL_Renderer* renderer, const Uint8* keys, std::vector<SDL_Rect>& edible, std::vector<SDL_Rect>& walkable, Mix_Chunk* eat_sound, int index);
	std::shared_ptr<Ghost> makeGhost(std::shared_ptr<Texture_Manager> texture_manager, SDL_Renderer *renderer, std::vector<SDL_Rect> &walkable, enum class GhostType type);
	std::shared_ptr<Player> targetClosestPlayer(std::shared_ptr<Ghost> ghost, std::vector<std::shared_ptr<Player>> &players);
	int totalScore = 0;

};
#endif