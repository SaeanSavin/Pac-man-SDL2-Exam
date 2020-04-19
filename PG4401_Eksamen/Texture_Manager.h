#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <string>

class Texture_Manager {

public:
	Texture_Manager();
	SDL_Texture *draw(SDL_Renderer *r, SDL_Surface *s);
	SDL_Rect setCoords(SDL_Surface *s);
	SDL_Texture *loadTexture(const char *c, SDL_Renderer *r);

	void printFromTiles(std::string name, SDL_Renderer *r, SDL_Texture *t, SDL_Rect &dst, SDL_Rect &src);
	void printPlayerScore(int s, SDL_Renderer *r, SDL_Texture *t, SDL_Rect &dst, SDL_Rect &src);

};
#endif
