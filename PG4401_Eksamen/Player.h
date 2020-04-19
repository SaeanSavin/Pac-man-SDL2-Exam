#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Character.h"

class Player : public Character
{
public:
	//constructors
	Player(SDL_Texture *t, SDL_Renderer *r, const Uint8 *k, std::vector<SDL_Rect> &p, SDL_GameController *controller, std::vector<SDL_Rect> &w, int i);
	Player(SDL_Texture *t, SDL_Rect c, SDL_Renderer *r, const Uint8 *k, std::vector<SDL_Rect> &p, SDL_GameController *controller, std::vector<SDL_Rect> &w, int i);

	//movement
	void move(int &SCREEN_WIDTH, int &SCREEN_HEIGHT, std::vector<std::vector<char>> &map, std::vector<SDL_Rect> &walls) override;

	//collision
	bool checkWallCollision(std::vector<SDL_Rect> &walls, int xOffset, int yOffset);

	bool checkEdibleCollision(std::vector<SDL_Rect> &edible, std::vector<std::vector<char>> &map);

	bool checkTileEntered(std::vector<SDL_Rect> &walkable);

	//animation 
	void renderTexture();

	void animate(Animation a);

	void animateEx(Animation a, char direction);

	void setAnimation(std::string name, std::shared_ptr<Animation> animation) {
		animations[name] = animation;
	}

	//sound
	void setSound(Mix_Chunk *s) {
		sound = s;
	}

	void playSound() {
		if (sound != nullptr && !Mix_Playing(2)) {
			Mix_PlayChannel(2, sound, -1);
		}
	}

	void stopSound() {
		if (Mix_Playing(2)) {
			Mix_HaltChannel(2);
		}
	}

	SDL_Texture* getTexture() override;

	SDL_Rect* getCoords() override;

	//Getters and Setters
	void setPos(int x, int y) override;

	void setSpawnPos(int x, int y) override;

	void setSize(int h, int w) override;

	char getDirection() override;

	void setDirection(char d) {
		direction = d;
	}

	int getScore() {
		return score;
	}

	int getHP() {
		return hp;

	}

	//other
	void respawn() override;

	bool isPowered() {
		return powered;
	}

	void addScore(int s) {
		score += s;
	}
	
	void stopPowered() {
		powered = false;
	}

	void hitByGhost() {
		hp--;
		Animation a = *animations["dead"];
		animations["dead"]->resetAnimation();
		int length = a.getLength();
		int rate = a.getRate();
		SDL_Delay(rate * 3);
		for (int i = 0; i < length - 3; i++) {
			animate(*animations["dead"]);
			SDL_RenderPresent(renderer);
			SDL_Delay(rate);
		}
		respawn();
	}

private:
	const Uint8* keys;
	SDL_GameController *gameController = nullptr;
	std::pair<int, int> spawn;
	std::vector<SDL_Rect> &pellets;
	std::vector<SDL_Rect> &walkable;
	
	std::map<std::string, std::shared_ptr<Animation>> animations;
	
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Rect coords;

	Mix_Chunk* sound = nullptr;
	
	char direction = ' ';
	char nextDirection = 'n';
	char collided = 'n';
	
	int speed = 1;
	int score = 0;
	int hp = 3;
	int pIndex = 0;

	bool powered = false;
};
#endif