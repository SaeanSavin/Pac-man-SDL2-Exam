#include "Texture_Manager.h"

Texture_Manager::Texture_Manager() {}

SDL_Texture *Texture_Manager::draw(SDL_Renderer *r, SDL_Surface *s) {
	SDL_Texture *texture = SDL_CreateTextureFromSurface(r, s);
	return texture;
}

SDL_Rect Texture_Manager::setCoords(SDL_Surface *s) {
	SDL_Rect coords;
	coords.h = 16;
	coords.w = 16;
	coords.x = 0;
	coords.y = 100;
	return coords;
}


