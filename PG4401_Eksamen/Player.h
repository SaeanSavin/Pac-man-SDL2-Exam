#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Character.h"

#include <iostream>
#include <vector>
#include <SDL.h>

class Player : public Character
{
public:
	Player(SDL_Texture* t);
	Player(SDL_Texture* t, SDL_Rect c);

	void movePlayer(const Uint8 *keys, SDL_Surface *surface, int &SCREEN_WIDTH, int &SCREEN_HEIGHT) override;

	SDL_Texture* getTexture() override;

	SDL_Rect* getCoords() override;

	void setPos(int x, int y) override;

	void setSize(int h, int w) override;

	char getDirection() {
		return direction;
	}
	void setDirection(char d) {
		direction = d;
	}

	bool checkWallCollision(std::vector<SDL_Rect> &walls);

private:
	SDL_Texture* texture;
	SDL_Rect coords;
	char direction = 'i';
	char collided = 'n';
	int speed = 2;
};

#endif