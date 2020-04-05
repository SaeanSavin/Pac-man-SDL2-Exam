#include "Player.h"

//movement function
void Player::movePlayer(const Uint8 *keys, SDL_Rect &coords, SDL_Surface *surface, int &SCREEN_WIDTH, int &SCREEN_HEIGHT) {

	SDL_PumpEvents();
	//if idle, check input
	if (keys[SDL_SCANCODE_W]) {
		direction = 'w';
	}

	if (keys[SDL_SCANCODE_S]) {
		direction = 's';
	}

	if (keys[SDL_SCANCODE_A]) {
		direction = 'a';
	}

	if (keys[SDL_SCANCODE_D]) {
		direction = 'd';
	}

	//if out of bounds, set to idle
	else {

		//Make sure image dont move out of bounds
		if (coords.x > SCREEN_WIDTH - surface->w) {
			coords.x = SCREEN_HEIGHT - surface->w;
			direction = 'i';
		}

		if (coords.y > SCREEN_WIDTH - surface->h) {
			coords.y = SCREEN_HEIGHT - surface->h;
			direction = 'i';
		}

		if (coords.x < 0) {
			coords.x = 0;
			direction = 'i';
		}

		if (coords.y < 0) {
			coords.y = 0;
			direction = 'i';
		}
	}
	//decide movement
	switch (direction) {
	case 'w':
		coords.y -= speed;
		break;
	case 's':
		coords.y += speed;
		break;
	case 'a':
		coords.x -= speed;
		break;
	case 'd':
		coords.x += speed;
		break;
	default:
		break;
	}

	//std::cout << "coords: " << coords.x << ", " << coords.y << std::endl;
	//std::cout << surface->w << ", " << surface->h << std::endl;
}