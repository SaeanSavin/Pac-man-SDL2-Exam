#ifndef _SOUND_H_
#define _SOUND_H_

#include <iostream>
#include <SDL.h>

class Sound
{
public:
	Sound(std::string p, SDL_AudioSpec* wS, SDL_AudioDeviceID* dID, Uint8* wB);
	void play();
private:
	std::string path;
	SDL_AudioSpec* wavSpec;
	SDL_AudioDeviceID* deviceID;
	Uint8* wavBuffer;
};

#endif