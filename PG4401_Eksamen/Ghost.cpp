#include "Ghost.h"

Ghost::Ghost(SDL_Texture* t, SDL_Renderer* r) {
	texture = t;
	renderer = r;
	coords.h = 0;
	coords.w = 0;
	coords.x = 0;
	coords.y = 0;
}
Ghost::Ghost(SDL_Texture* t, SDL_Rect c, SDL_Renderer* r) {
	texture = t;
	renderer = r;
	coords = c;
}

void Ghost::movePlayer(const Uint8* keys, SDL_Surface* surface, int& SCREEN_WIDTH, int& SCREEN_HEIGHT, std::vector<std::vector<char>>& map, std::vector<SDL_Rect>& walls, std::vector<SDL_Rect>& pellets) {
}

SDL_Texture* Ghost::getTexture() {
	return texture;
}

SDL_Rect* Ghost::getCoords() {
	return &coords;
}

void Ghost::setPos(int x, int y) {
	coords.x = x;
	coords.y = y;
}

void Ghost::setSize(int h, int w) {
	coords.h = h;
	coords.w = w;
}

void Ghost::animate(Animation a) {
	SDL_RenderCopy(renderer, a.getFrame(), nullptr, &coords);
}

void Ghost::animateEx(Animation a, char direction) {
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	double rotation = 0;
	if (direction == 'a') {
		flipType = SDL_FLIP_HORIZONTAL;
	}
	else if (direction == 'w') {
		flipType = SDL_FLIP_VERTICAL;
		rotation = 270;
	}
	else if (direction == 's') {
		rotation = 90;
	}
	SDL_RenderCopyEx(renderer, a.getFrame(), nullptr, &coords, rotation, NULL, flipType);
}

void Ghost::renderTexture() {
	SDL_RenderCopy(renderer, texture, nullptr, &coords);
}

bool Ghost::checkWallCollision(std::vector<SDL_Rect>& walls, int x_offset, int y_offset) {
	int x = coords.x + x_offset;
	int y = coords.y + y_offset;
	for (auto& wall : walls) {
		if (y + coords.h > wall.y && y < wall.y + wall.h) {
			if (x + coords.w > wall.x && x < wall.x + wall.w) {
				if (direction != 'i') {
					collided = direction;
					renderTexture();
				}
				return true;
			}
		}
	}
	return false;
};
