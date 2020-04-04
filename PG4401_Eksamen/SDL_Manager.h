#ifndef _SDLMANAGER_H_
#define _SDL_MANAGER_H_

#include <SDL.h>
#include <SDL_image.h>

class SDL_Manager {

private:

	void printError(std::ostream &os, const std::string &msg);

public:
	SDL_Manager();

	SDL_Window *createWindow(const char * c, const int w, const int h);
	SDL_Renderer *createRenderer(SDL_Window *w);

};


#endif
