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

void Ghost::move(const Uint8 *keys, SDL_Surface *surface, int &SCREEN_WIDTH, int &SCREEN_HEIGHT, std::vector<std::vector<char>> &map, std::vector<SDL_Rect> &walls, std::vector<SDL_Rect> &walkable, std::pair<int, int> target) {
	//checkWallCollision(walls, 0, 0);

	SDL_PumpEvents();


	//calculate direction based on target

	if (checkTileEntered(walkable)) {

		double closestDir = 0;
		int newDir = 0;
		char newDirection = direction;
		if (direction != 'd' && collided != 'a' && !checkWallCollision(walls, -1, 0)) {
			closestDir = std::pow(abs((coords.x - 16) - target.first),2) + std::pow(abs((coords.y) - target.second),2);
			newDirection = 'a';
			collided = 'n';
			std::cout << "set direction to a, collided: " << collided << std::endl;
		}

		if (direction != 'w' && collided != 's' && !checkWallCollision(walls, 0, 1)) {
			collided = 'n';
			newDir = std::pow(abs((coords.x) - target.first), 2) + std::pow(abs((coords.y + 16) - target.second), 2);
			if (newDir <= closestDir) {
				closestDir = newDir;
				newDirection = 's';
			}
		}

		if (direction != 'a' && collided != 'd' && !checkWallCollision(walls, 1, 0)) {
			collided = 'n';
			newDir = std::pow(abs((coords.x + 16) - target.first), 2) + std::pow(abs((coords.y) - target.second), 2);
			if (newDir <= closestDir) {
				closestDir = newDir;
				newDirection = 'd';
				std::cout << "set direction to d" << std::endl;
			}
		}

		if (direction != 's' && collided != 'w' && !checkWallCollision(walls, 0, -1)) {
			collided = 'n';
			newDir = std::pow(abs((coords.x) - target.first), 2) + std::pow(abs((coords.y + 16) - target.second), 2);
			if (newDir <= closestDir) {
				closestDir = newDir;
				newDirection = 'w';
			}
		}
		direction = newDirection;

		std::cout << "chose: " << newDirection
			<< ", because left was " << std::pow(abs((coords.x - 16) - target.first), 2) + std::pow(abs((coords.y) - target.second), 2)
			<< ", and right was: " << std::pow(abs((coords.x + 16) - target.first), 2) + std::pow(abs((coords.y) - target.second), 2);
		
		std::cout << "closest: " << closestDir << std::endl;
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
			animate(*animations["up"]);
		}
		collided = 'n';
		break;
	case 's':
		if (!checkWallCollision(walls, 0, 1)) {
			coords.y += speed;
			animate(*animations["down"]);
		}
		collided = 'n';
		break;
	case 'a':
		if (!checkWallCollision(walls, -1, 0)) {
			coords.x -= speed;
			animate(*animations["left"]);
		}
		collided = 'n';
		break;
	case 'd':
		if (!checkWallCollision(walls, 1, 0)) {
			coords.x += speed;
			animate(*animations["right"]);
		}
		collided = 'n';
		break;
	case 'i':
		renderTexture();
	default:
		break;
	}
}

bool Ghost::checkTileEntered(std::vector<SDL_Rect>& map) {

	for (auto& mapTile : map) {
		if (coords.y == mapTile.y && coords.x == mapTile.x) {
			return true;
		}
	}
	return false;
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
					direction = 'i';
					renderTexture();
				}
				std::cout << "collided" << std::endl;
				return true;
			}
		}
	}
	return false;
};
