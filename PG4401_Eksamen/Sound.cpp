#include "Sound.h"

Sound::Sound(std::string p, SDL_AudioSpec* wS, SDL_AudioDeviceID* dID, Uint8* wB) {
	path = p;
	wavSpec = wS;
	deviceID = dID;
	wavBuffer = wB;
}

void Sound::play() {
	Uint32 wavLength;

	SDL_LoadWAV("Powerup5.wav", wavSpec, &wavBuffer, &wavLength);

	SDL_QueueAudio(*deviceID, wavBuffer, wavLength);
	SDL_PauseAudioDevice(*deviceID, 0);
}