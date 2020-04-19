#include "Animation.h"

Animation::Animation(SDL_Renderer* r, std::string path, int tickRate) {
	length = 0;
	rate = tickRate;
	startTime = SDL_GetTicks();
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		length++;
		auto tm = std::make_unique<Texture_Manager>();
		if (entry.path().has_extension()) {
			frames.emplace_back(tm->loadTexture(&(entry.path().string())[0], r));
		}
	}
}

SDL_Texture* Animation::getFrame() {
	int frame = ((SDL_GetTicks() - startTime) * rate / 1000) % length;
	return frames[frame];
}

void Animation::resetAnimation() {
	startTime = SDL_GetTicks();
}