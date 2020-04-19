#ifndef _GHOST_H_
#define _GHOST_H_

#include "Character.h"

enum class TargetType {
	AGRESSIVE,
	SUPPORTIVE,
	AMBUSH,
	EVASIVE,
	SCATTER,
	FRIGHTENED,
	RETURN
};

enum class GhostType {
	SHADOW,
	SPEEDY,
	BASHFUL,
	POKEY
};

class Ghost : public Character
{
public:
	//constructors
	Ghost(SDL_Texture* t, SDL_Renderer* r, std::vector<SDL_Rect>& w, enum class TargetType m, enum class GhostType g);
	Ghost(SDL_Texture* t, SDL_Renderer* r, SDL_Rect c, std::vector<SDL_Rect>& w, enum class TargetType m, enum class GhostType g);

	//movement
	void move(int& SCREEN_WIDTH, int& SCREEN_HEIGHT, std::vector<std::vector<char>>& map, std::vector<SDL_Rect>& walls) override;

	bool checkTileEntered(std::vector<SDL_Rect>& walkable);

	bool isHome(std::vector<SDL_Rect>& walkable, std::vector<std::vector<char>>& map);

	//collision
	bool checkWallCollision(std::vector<SDL_Rect>& walls, int x_offset, int y_offset);

	//animation 
	void renderTexture();

	void animate(Animation a);

	void animateEx(Animation a, char direction);

	void setAnimation(std::string name, std::shared_ptr<Animation> animation) {
		animations[name] = animation;
	}

	//getters and setters
	SDL_Texture* getTexture() override;

	SDL_Rect* getCoords() override;

	void setPos(int x, int y) override;

	void setSize(int h, int w) override;

	void setTarget(std::pair<int, int> t);

	char getDirection() override {
		return direction;
	}
	void setDirection(char d) {
		direction = d;
	}

	void setSpawnPos(int x, int y) override;

	std::pair<int, int> getSpawnPos();

	void respawn() override;

	enum class GhostType getType() {
		return type;
	}

	enum class TargetType getTargetMode();

	void startFrightened() {
		frightened = true;
	}

	void startFrightenedEnding() {
		frightenedEnding = true;
	}

	void stopFrightenedEnding() {
		frightenedEnding = false;
	}

	void stopFrightened() {
		frightenedEnding = false;
		frightened = false;
	}

	void startEaten() {
		eaten = true;
	}

	void stopEaten() {
		eaten = false;
	}

	void hitByPacman() {
		frightenedEnding = false;
		frightened = false;
		eaten = true;
	}

	bool isEaten() {
		return eaten;
	}

	bool isFrightened() {
		return frightened;
	}

private:
	enum class TargetType targetMode;
	enum class GhostType type;

	std::vector<SDL_Rect> walkable;
	SDL_Texture* texture;
	SDL_Rect coords;
	SDL_Renderer* renderer;

	std::pair<int, int> spawn;
	std::pair<int, int> target;

	std::map<std::string, std::shared_ptr<Animation>> animations;
	
	char direction = 'i';
	char nextDirection = 'n';
	char collided = 'n';
	
	int speed = 1;

	bool frightened = false;
	bool frightenedEnding = false;
	bool eaten = false;
};

#endif _GHOST_H_