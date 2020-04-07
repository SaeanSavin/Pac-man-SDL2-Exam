#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <SDL.h>

class Character
{
public:
	virtual void movePlayer(const Uint8* keys, SDL_Rect& coords, SDL_Surface* surface, int& SCREEN_WIDTH, int& SCREEN_HEIGHT) = 0;
private:
	SDL_Texture* texture;
	int speed = 0;
	//TODO: add move function

};
#endif