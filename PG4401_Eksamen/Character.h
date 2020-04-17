#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <iostream>
#include <vector>
#include <map>
#include <SDL.h>

#include "Animation.h"
class Character
{
public:

	virtual SDL_Texture* getTexture() = 0;

	virtual SDL_Rect* getCoords() = 0;

	virtual void setPos(int x, int y) = 0;

	virtual void setSize(int h, int w) = 0;

	virtual char getDirection() = 0;

	virtual void setAnimation(std::string name, std::shared_ptr<Animation> animation) = 0;

	virtual int getScore() = 0;

	virtual void hitByGhost() = 0;

	virtual int getHP() = 0;

	virtual void move(SDL_Surface* surface, int& SCREEN_WIDTH, int& SCREEN_HEIGHT, std::vector<std::vector<char>>& map, std::vector<SDL_Rect>& walls) = 0;

	virtual void setSpawnPos(int x, int y) = 0;

	virtual void respawn() = 0;

private:
	int speed = 2;
	//TODO: add move function

};
#endif