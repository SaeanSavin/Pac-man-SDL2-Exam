#include "Player.h"

Player::Player(SDL_Texture* t) {
	texture = t;
	coords.h = 0;
	coords.w = 0;
	coords.x = 0;
	coords.y = 0;
}
Player::Player(SDL_Texture* t, SDL_Rect c) {
	texture = t;
	coords = c;
}

//movement function
void Player::movePlayer(const Uint8 *keys, SDL_Surface *surface, int &SCREEN_WIDTH, int &SCREEN_HEIGHT, std::vector<SDL_Rect>& walls) {

	checkWallCollision(walls, 0, 0);

	SDL_PumpEvents();

	//check if we can go to the next direction from user input
	if (next_direction != 'n' && next_direction != direction) {
		if (next_direction == 'w' && collided != 'w') {
			if (!checkWallCollision(walls, 0, -1)) {
				direction = 'w';
			}
		}

		if (next_direction == 's' && collided != 's') {
			if (!checkWallCollision(walls, 0, 1)) {
				direction = 's';
			}
		}

		if (next_direction == 'a' && collided != 'a') {
			if (!checkWallCollision(walls, -1, 0)) {
				direction = 'a';
			}
		}

		if (next_direction == 'd' && collided != 'd') {
			if (!checkWallCollision(walls, 1, 0)) {
				direction = 'd';
			}
		}
	}

	//if idle, check input
	if (keys[SDL_SCANCODE_W] && collided != 'w') {
		if (!checkWallCollision(walls, 0, -1)) {
			direction = 'w';
			next_direction = 'n';
		}
		else {
			next_direction = 'w';
		}
	}

	if (keys[SDL_SCANCODE_S] && collided != 's') {
		if (!checkWallCollision(walls, 0, 1)) {
			direction = 's';
			next_direction = 'n';
		}
		else {
			next_direction = 's';
		}
	}

	if (keys[SDL_SCANCODE_A] && collided != 'a') {
		if (!checkWallCollision(walls, -1, 0)) {
			direction = 'a';
			next_direction = 'n';
		}
		else {
			next_direction = 'a';
		}
	}

	if (keys[SDL_SCANCODE_D] && collided != 'd') {
		if (!checkWallCollision(walls, 1, 0)) {
			direction = 'd';
			next_direction = 'n';
		}
		else {
			next_direction = 'd';
		}
	}

	//if out of bounds, set to idle
	else {

		//Make sure image dont move out of bounds
		if (coords.x > SCREEN_WIDTH - coords.w) {
			coords.x = SCREEN_WIDTH - coords.w;
			direction = 'i';
		}

		if (coords.y > SCREEN_HEIGHT - coords.h) {
			coords.y = SCREEN_HEIGHT - coords.h;
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
		if (!checkWallCollision(walls, 0, -1)) {
			coords.y -= speed;
		}
		collided = 'n';
		break;
	case 's':
		if (!checkWallCollision(walls, 0, 1)) {
			coords.y += speed;
		}
		collided = 'n';
		break;
	case 'a':
		if (!checkWallCollision(walls, -1, 0)) {
			coords.x -= speed;
		}
		collided = 'n';
		break;
	case 'd':
		if (!checkWallCollision(walls, 1, 0)) {
			coords.x += speed;
		}
		collided = 'n';
		break;
	default:
		break;
	}
}

SDL_Texture* Player::getTexture() {
	return texture;
}

SDL_Rect* Player::getCoords() {
	return &coords;
}

void Player::setPos(int x, int y) {
	coords.x = x;
	coords.y = y;
}

void Player::setSize(int h, int w) {
	coords.h = h;
	coords.w = w;
}
bool Player::checkWallCollision(std::vector<SDL_Rect>& walls, int x_offset, int y_offset) {
	int x = coords.x + x_offset;
	int y = coords.y + y_offset;
	for (auto &wall : walls) {
		if (y + coords.h > wall.y && y < wall.y + wall.h) {
			if (x + coords.w > wall.x && x < wall.x + wall.w) {
				if (direction != 'i') {
					collided = direction;
				}
				std::cout << "collided: " << collided << std::endl;
				return true;
			}
		}
	}
	std::cout << "direction: " << direction << std::endl;
	return false;
};