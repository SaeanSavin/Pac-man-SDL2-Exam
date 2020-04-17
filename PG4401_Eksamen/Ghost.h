#ifndef _GHOST_H_
#define _GHOST_H_

#include "Character.h"

enum class TargetType {
	AGRESSIVE,
	SUPPORTIVE,
	AMBUSH,
	EVASIVE,
};

class Ghost : public Character
{
public:
	//constructors
	Ghost(SDL_Texture* t, SDL_Renderer* r, std::vector<SDL_Rect>& w, enum class TargetType m);
	Ghost(SDL_Texture* t, SDL_Renderer* r, SDL_Rect c, std::vector<SDL_Rect>& w, enum class TargetType m);

	//movement
	void move(SDL_Surface* surface, int& SCREEN_WIDTH, int& SCREEN_HEIGHT, std::vector<std::vector<char>>& map, std::vector<SDL_Rect>& walls) override;

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

	void respawn() override;

	enum class TargetType getTargetMode();

	//virtual implementations
	void hitByGhost() override {};
	int getScore() override { return 0; };
	int getHP() override { return 0; };

private:
	enum class TargetType targetMode;
	std::vector<SDL_Rect> walkable;
	std::pair<int, int> spawn;
	std::pair<int, int> target;
	SDL_Renderer* renderer;
	std::map<std::string, std::shared_ptr<Animation>> animations;
	SDL_Texture* texture;
	SDL_Rect coords;
	char direction = 'i';
	char next_direction = 'n';
	char collided = 'n';
	int speed = 1;
};

#endif _GHOST_H_