#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <SDL.h>

#include <iostream>
#include <filesystem>

#include "Texture_Manager.h"

class Animation {
public:
	Animation(SDL_Renderer *r, std::string path, int tick_rate);
	SDL_Texture* getFrame();
	void resetAnimation();

	int getRate() {
		return rate;
	}
	int getLength() {
		return length;
	}

private:
	int rate;
	int length;
	Uint32 startTime;
	std::vector<SDL_Texture*> frames;
};

#endif
