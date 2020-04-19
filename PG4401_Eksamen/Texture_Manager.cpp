#include "Texture_Manager.h"

Texture_Manager::Texture_Manager() {}

SDL_Texture *Texture_Manager::draw(SDL_Renderer *r, SDL_Surface *s) {
	SDL_Texture *texture = SDL_CreateTextureFromSurface(r, s);
	return texture;
}

SDL_Rect Texture_Manager::setCoords(SDL_Surface *s) {
	SDL_Rect coords;
	coords.h = 22;
	coords.w = 22;
	coords.x = 0;
	coords.y = 0;
	return coords;
}

SDL_Texture *Texture_Manager::loadTexture(const char *c, SDL_Renderer *r) {
	SDL_Surface *tmpSurface = IMG_Load(c);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(r, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	return texture;
}

void Texture_Manager::printFromTiles(std::string name, SDL_Renderer *r, SDL_Texture *t, SDL_Rect &dst, SDL_Rect &src) {
	for (size_t i = 0; i < name.length(); i++) {

		if (name[i] == ' ') {
			dst.x += 16;
			continue;
		}

		if (name[i] == '!') {
			src.x = 11 * 8;
			src.y = 1 * 8;
			SDL_RenderCopy(r, t, &src, &dst);
			dst.x += 16;
			continue;
		}

		int x = name[i] - 65;
		int y = 0;

		if (x > 14) {
			x -= 15;
			y = 1;
		}

		src.x = x * 8;
		src.y = y * 8;
		SDL_RenderCopy(r, t, &src, &dst);
		dst.x += 16;
	}
}

void Texture_Manager::printPlayerScore(int score, SDL_Renderer *r, SDL_Texture *t, SDL_Rect &dst, SDL_Rect &src) {
	
	src.y = 2 * 8;
	int digit = score / 10;
	std::vector<int> digits{};

	while (score > 9) {

		digits.emplace_back(digit % 10);
		printPlayerScore(digit, r, t, dst, src);
		break;
	}

	for (size_t i = 0; i < digits.size(); i++) {
		src.x = digits[i] * 8;
		SDL_RenderCopy(r, t, &src, &dst);
		dst.x += 16;
	}
	src.x = 0;
	SDL_RenderCopy(r, t, &src, &dst);
}