#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include <SDL.h>
#include <SDL_image.h>

class Texture_Manager {

public:
	Texture_Manager();
	SDL_Texture *draw(SDL_Renderer *r, SDL_Surface *s);
	SDL_Rect setCoords(SDL_Surface *s);

private:
	
};
#endif
