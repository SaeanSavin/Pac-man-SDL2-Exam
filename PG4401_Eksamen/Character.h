#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <SDL.h>
#include <vector>

class Character
{
public:

	virtual void movePlayer(const Uint8* keys, SDL_Surface* surface, int& SCREEN_WIDTH, int& SCREEN_HEIGHT, std::vector<std::vector<char>> &map, std::vector<SDL_Rect> &walls, std::vector<SDL_Rect> &pellets) = 0;

	virtual SDL_Texture* getTexture() = 0;

	virtual SDL_Rect* getCoords() = 0;

	virtual void setPos(int x, int y) = 0;

	virtual void setSize(int h, int w) = 0;
private:
	int speed = 2;
	//TODO: add move function

};
#endif