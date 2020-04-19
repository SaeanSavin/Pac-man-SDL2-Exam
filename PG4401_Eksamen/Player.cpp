#include "Player.h"

Player::Player(SDL_Texture* t, SDL_Renderer* r, const Uint8* k, std::vector<SDL_Rect>& p, SDL_GameController * controller, std::vector<SDL_Rect>& w)
	:texture(t), renderer(r), keys(k), pellets(p), gameController(controller), walkable(w)
{
	coords.h = 0;
	coords.w = 0;
	coords.x = 0;
	coords.y = 0;
}
Player::Player(SDL_Texture* t, SDL_Rect c, SDL_Renderer* r, const Uint8* k, std::vector<SDL_Rect>& p, SDL_GameController *controller, std::vector<SDL_Rect>& w)
	:texture(t), renderer(r), coords(c), keys(k), pellets(p), gameController(controller), walkable(w)
{}

//movement function
void Player::move(int& SCREEN_WIDTH, int& SCREEN_HEIGHT, std::vector<std::vector<char>>& map, std::vector<SDL_Rect>& walls) {

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

	//check input
	if ((keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP] || SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_UP)) && collided != 'w') {
		if (!checkWallCollision(walls, 0, -1)) {
			direction = 'w';
			next_direction = 'n';
		}
		else {
			next_direction = 'w';
		}
	}

	if ((keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN] || SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) && collided != 's') {
		if (!checkWallCollision(walls, 0, 1)) {
			direction = 's';
			next_direction = 'n';
		}
		else {
			next_direction = 's';
		}
	}

	if ((keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT] || SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) && collided != 'a') {
		if (!checkWallCollision(walls, -1, 0)) {
			direction = 'a';
			next_direction = 'n';
		}
		else {
			next_direction = 'a';
		}
	}

	if ((keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT] || SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) && collided != 'd') {
		if (!checkWallCollision(walls, 1, 0)) {
			direction = 'd';
			next_direction = 'n';
		}
		else {
			next_direction = 'd';
		}
	}

	//if out of bounds, re-enter on the opposite side
	else {

		//Make sure image dont move out of bounds
		if (coords.x > SCREEN_WIDTH && direction == 'd') {
			coords.x = -coords.w;
		}

		if (coords.y > SCREEN_HEIGHT && direction == 's') {
			coords.y = -coords.h;
		}

		if (coords.x < -coords.w && direction == 'a') {
			coords.x = SCREEN_WIDTH;
		}

		if (coords.y < -coords.h && direction == 'w') {
			coords.y = SCREEN_HEIGHT;
		}
	}
	//decide movement
	switch (direction) {
	case 'w':
		if (!checkWallCollision(walls, 0, -1)) {
			coords.y -= speed;
			animateEx(*animations["move"], 'w');
		}
		collided = 'n';
		break;
	case 's':
		if (!checkWallCollision(walls, 0, 1)) {
			coords.y += speed;
			animateEx(*animations["move"], 's');
		}
		collided = 'n';
		break;
	case 'a':
		if (!checkWallCollision(walls, -1, 0)) {
			coords.x -= speed;
			animateEx(*animations["move"], 'a');
		}
		collided = 'n';
		break;
	case 'd':
		if (!checkWallCollision(walls, 1, 0)) {
			coords.x += speed;
			animate(*animations["move"]);
		}
		collided = 'n';
		break;
	case ' ':
		renderTexture();
	default:
		break;
	}
	if (checkEdibleCollision(pellets, map)) {
		playSound();
	}
	else if (checkTileEntered(walkable)) {
		stopSound();
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

char Player::getDirection() {
	return direction;
}

void Player::setSpawnPos(int x, int y) {
	spawn.first = x;
	spawn.second = y;
	setPos(x, y);
}

void Player::respawn() {
	setPos(spawn.first, spawn.second);
}

void Player::animate(Animation a) {
	SDL_RenderCopy(renderer, a.getFrame(), nullptr, &coords);
}

void Player::animateEx(Animation a, char direction) {
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

void Player::renderTexture() {
	SDL_RenderCopy(renderer, texture, nullptr, &coords);
}

bool Player::checkWallCollision(std::vector<SDL_Rect>& walls, int x_offset, int y_offset) {
	int x = coords.x + x_offset;
	int y = coords.y + y_offset;
	for (auto &wall : walls) {
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

bool Player::checkEdibleCollision(std::vector<SDL_Rect>& edible, std::vector<std::vector<char>> &map) {

	for (auto& e : edible) {
		if (coords.y == e.y) {
			if (coords.x == e.x && map[(e.y - 50) / 16][e.x / 16] == 'x') {
				map[(e.y - 50) / 16][e.x / 16] = ' ';
				score+= 10;
				return true;
			}

			else if (coords.x == e.x && map[(e.y - 50) / 16][e.x / 16] == 'C') {
				map[(e.y - 50) / 16][e.x / 16] = ' ';
				score += 100;
				return true;
			}

			else if (coords.x == e.x && map[(e.y - 50) / 16][e.x / 16] == 'A') {
				map[(e.y - 50) / 16][e.x / 16] = ' ';
				score += 200;
				return true;
			}
			
			else if (coords.x == e.x && map[(e.y - 50) / 16][e.x / 16] == 'P') {
				map[(e.y - 50) / 16][e.x / 16] = ' ';
				powered = true;
				return true;
			}
		}
	}
	return false;
}

bool Player::checkTileEntered(std::vector<SDL_Rect>& walkable) {

	for (auto& mapTile : walkable) {
		if (coords.y == mapTile.y && coords.x == mapTile.x) {
			return true;
		}
	}
	return false;
}