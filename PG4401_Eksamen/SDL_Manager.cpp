#include <iostream>

#include "SDL_Manager.h"

void SDL_Manager::printError(std::ostream &os, const std::string &msg) {
	os << msg << " error " << SDL_GetError() << std::endl;
}

SDL_Manager::SDL_Manager() {}

SDL_Window *SDL_Manager::createWindow(const char *c, const int w, const int h) {
	
	SDL_Window *window = SDL_CreateWindow(c,
										  SDL_WINDOWPOS_UNDEFINED,
										  SDL_WINDOWPOS_UNDEFINED,
										  w,
										  h,
										  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS
										  );
	if (window == nullptr) {
		printError(std::cout, "Cannot create window ");
		SDL_Quit();
		return nullptr;
	}

	return window;
}

SDL_Renderer *SDL_Manager::createRenderer(SDL_Window *w) {
	SDL_Renderer *renderer = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr) {
		printError(std::cout, "Cannot create renederer ");
		SDL_DestroyWindow(w);
		SDL_Quit();
		return nullptr;
	}

	return renderer;
}



