#ifndef _GHOST_H_
#define _GHOST_H_

#include "Character.h"

#include <iostream>
#include <vector>
#include <SDL.h>

#include "Animation.h"

class Ghost : public Character
{
public:
	//constructors
	Ghost(SDL_Texture* t, SDL_Renderer* r);
	Ghost(SDL_Texture* t, SDL_Rect c, SDL_Renderer* r);

	//movement
	void move(const Uint8* keys, SDL_Surface* surface, int& SCREEN_WIDTH, int& SCREEN_HEIGHT, std::vector<std::vector<char>>& map, std::vector<SDL_Rect>& walls, std::vector<SDL_Rect>& pellets) override;

	//collision
	bool checkWallCollision(std::vector<SDL_Rect>& walls, int x_offset, int y_offset);

	//animation 
	void renderTexture();

	void animate(Animation a);

	void animateEx(Animation a, char direction);

	void setAnimation(std::string name, std::shared_ptr<Animation> animation) {
		animations[name] = animation;
	}

	//getters and setters
	SDL_Texture* getTexture() override;

	SDL_Rect* getCoords() override;

	void setPos(int x, int y) override;

	void setSize(int h, int w) override;

	char getDirection() {
		return direction;
	}
	void setDirection(char d) {
		direction = d;
	}

private:
	SDL_Renderer* renderer;
	std::map<std::string, std::shared_ptr<Animation>> animations;
	SDL_Texture* texture;
	SDL_Rect coords;
	char direction = 'i';
	char next_direction = 'n';
	char collided = 'n';
	int speed = 1;
};

#endif _GHOST_H_
