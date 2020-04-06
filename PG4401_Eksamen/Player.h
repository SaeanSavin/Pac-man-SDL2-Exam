#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Character.h"

#include <iostream>
#include <SDL.h>

class Player : public Character
{
public:
	void movePlayer(const Uint8 *keys, SDL_Rect &coords, SDL_Surface *surface, int &SCREEN_WIDTH, int &SCREEN_HEIGHT) override;

	char getDirection() {
		return direction;
	}
	void setDirection(char d) {
		direction = d;
	}
private:
	int speed = 2;
	char direction;
};

#endif