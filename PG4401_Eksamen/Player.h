#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Character.h"

#include <SDL.h>

class Player :
	public Character
{
public:
	char movePlayer(const Uint8* keys, SDL_Rect& coords, SDL_Surface* surface, char direction);
};

#endif