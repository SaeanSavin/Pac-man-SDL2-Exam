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

	void movePlayer(const Uint8 *keys, SDL_Surface *surface, int &SCREEN_WIDTH, int &SCREEN_HEIGHT, std::vector<std::vector<char>>& map, std::vector<SDL_Rect>& walls, std::vector<SDL_Rect>& pellets) override;

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
	int getScore() {
		return score;
	}

	bool checkWallCollision(std::vector<SDL_Rect> &walls, int x_offset, int y_offset);

	bool checkPelletCollision(std::vector<SDL_Rect> &pellets, std::vector<std::vector<char>> &map);

private:
	SDL_Texture* texture;
	SDL_Rect coords;
	char direction = 'i';
	char next_direction = 'n';
	char collided = 'n';
	int speed = 1;
	int score = 0;
};

#endif