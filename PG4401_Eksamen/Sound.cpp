#include "Sound.h"

Sound::Sound(std::string p, SDL_AudioSpec* wS, SDL_AudioDeviceID* dID) {
	path = p;
	wavSpec = wS;
	deviceID = dID;
}

void Sound::play() {
	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8* wavBuffer;

	SDL_LoadWAV("Powerup5.wav", &wavSpec, &wavBuffer, &wavLength);

	SDL_QueueAudio(*deviceID, wavBuffer, wavLength);
	SDL_PauseAudioDevice(*deviceID, 0);
}