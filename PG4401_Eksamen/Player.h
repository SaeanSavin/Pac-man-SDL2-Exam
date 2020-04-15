#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Character.h"

#include <iostream>
#include <vector>
#include <SDL.h>

#include "Animation.h"

class Player : public Character
{
public:
	//constructors
	Player(SDL_Texture* t, SDL_Renderer* r);
	Player(SDL_Texture* t, SDL_Rect c, SDL_Renderer* r);

	//movement
	void move(const Uint8 *keys, SDL_Surface *surface, int &SCREEN_WIDTH, int &SCREEN_HEIGHT, std::vector<std::vector<char>>& map, std::vector<SDL_Rect>& walls, std::vector<SDL_Rect>& pellets);

	//collision
	bool checkWallCollision(std::vector<SDL_Rect>& walls, int x_offset, int y_offset);

	bool checkPelletCollision(std::vector<SDL_Rect>& pellets, std::vector<std::vector<char>>& map);

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

	void setSpawnPos(int x, int y);

	void setSize(int h, int w) override;

	char getDirection() {
		return direction;
	}
	void setDirection(char d) {
		direction = d;
	}
	int getScore() {
		return score;
	}
	int getHP() {
		return hp;
	}

	SDL_Rect getSpawnPos() {
		return startCoords;
	}

	void hitByGhost() {
		hp--;
		coords.x = startCoords.x;
		coords.y = startCoords.y;
	}

private:
	
	std::map<std::string, std::shared_ptr<Animation>> animations;
	
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Rect coords;
	SDL_Rect startCoords;
	
	char direction = ' ';
	char next_direction = 'n';
	char collided = 'n';
	
	int speed = 1;
	int score = 0;
	int hp = 3;
};

#endif