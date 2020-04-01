#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

//Window size
const int SCREEN_WIDTH = 450;
const int SCREEN_HEIGHT = 500;

//Player speed
const int speed = 4;

//Error function
void printError(std::ostream &os, const std::string &msg) {
	os << msg << " error " << SDL_GetError() << std::endl;
}

//movement function
char movePlayer(const Uint8 *keys, SDL_Rect &coords, SDL_Surface *surface, char direction) {
	
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
			coords.x = SCREEN_WIDTH - surface->w;
			direction = 'i';
		}

		if (coords.y > SCREEN_HEIGHT - surface->h) {
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
	
	return direction;
}

void frames(const int FPS) {
	
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	frameStart = SDL_GetTicks();

	frameTime = SDL_GetTicks() - frameStart;

	if (frameDelay > frameTime) {
		SDL_Delay(frameDelay - frameTime);
	}
}

SDL_Window *createWindow(const int w, const int h) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = nullptr;

	//Creates Window
	window = SDL_CreateWindow(

		"Pac-man",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		w,
		h,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS
	);
	return window;
}

SDL_Texture* LoadTexture(const char *pos, SDL_Renderer *renderer) {

	SDL_Surface* tmpSurface = IMG_Load(pos);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	return texture;
}

void loadMap(std::string map, std::vector<std::vector<char>>& mapVector) {
	std::ifstream fIn(map);

	if (!fIn.is_open()) {
		std::cout << "Cannot open map file" << std::endl;
		return;
	}

	std::string str;

	while (std::getline(fIn, str)) {

		std::vector<char> mapRow;

		for (int i = 0; i < str.length(); i++) {
			mapRow.emplace_back(str[i]);
		}

		mapVector.emplace_back(mapRow);
	}

	fIn.close();
}

void drawMap(std::vector<std::vector<char>>& mapVector) {

}

int main(int argc, char *argv[]) {
	
	const int FPS = 60;

	std::vector<std::vector<char>> map {};
	loadMap("../maps/pacMap.txt", map);

	//for debug
	for (auto& row : map) {
		for (auto& c : row) {
			std::cout << c;
		}
		std::cout << std::endl;
	}

	SDL_Window *window = createWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (window == nullptr) {
		printError(std::cout, "Failed to create window: ");
		SDL_Quit();
		return EXIT_FAILURE;
	}


	//Creates Renderer 
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr) {
		printError(std::cout, "Failed to create renderer: ");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	//draws background 
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//Creates Surface
	SDL_Surface *surface = IMG_Load("../images/hamstercam.png");

	//Checks if a surface exist
	if (surface == nullptr) {
		printError(std::cout, "Failed to load image: ");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		return EXIT_FAILURE;
	}

	//Draw images to VRAM
	SDL_Texture *drawable = SDL_CreateTextureFromSurface(renderer, surface);

	//Setting coordinates for images
	SDL_Rect coords;
	coords.h = surface->h;
	coords.w = surface->w;
	coords.x = 0;
	coords.y = 100;

	//Freeing the RGB surface
	SDL_FreeSurface(surface);

	//Create Textures
	
	SDL_Texture* pellet = LoadTexture("../images/mapTiles/pellet.png", renderer);
	SDL_Texture* wall_bottom = LoadTexture("../images/mapTiles/wall_bottom_single.png", renderer);
	SDL_Texture* wall_top = LoadTexture("../images/mapTiles/wall_top_single.png", renderer);
	SDL_Texture* wall_left = LoadTexture("../images/mapTiles/wall_left_single.png", renderer);
	SDL_Texture* wall_right = LoadTexture("../images/mapTiles/wall_right_single.png", renderer);
	SDL_Texture* corner_top_right = LoadTexture("../images/mapTiles/wall_corner_tr_single.png", renderer);
	SDL_Texture* corner_top_left = LoadTexture("../images/mapTiles/wall_corner_tl_single.png", renderer);
	SDL_Texture* corner_bottom_right = LoadTexture("../images/mapTiles/wall_corner_br_single.png", renderer);
	SDL_Texture* corner_bottom_left = LoadTexture("../images/mapTiles/wall_corner_bl_single.png", renderer);

	bool isRunning = true;
	const Uint8 *keys = nullptr;
	int numKeys;
	keys = SDL_GetKeyboardState(&numKeys);
	SDL_Event evt;

	//movement variables
	char direction = 'i';

	//Game Loop
	while (isRunning) {

		frames(FPS);

		//Checks if Escape is press or X in the window
		if (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_KEYDOWN) {
				if (evt.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false;
				}
			} else if (evt.type == SDL_QUIT) {
				isRunning = false;
			}
		}

		//Keys input for movement
		direction = movePlayer(keys, coords, surface, direction);

		//std::cout << direction << std::endl;

		//Prepare Renderer for a new frame
		SDL_RenderCopy(renderer, drawable, nullptr, &coords);
		
		//render map

		SDL_Rect mapRect;
		mapRect.w = 16;
		mapRect.h = 16;
		mapRect.x = 0;
		mapRect.y = 0;



		for (auto& row : map) {
			for (auto& c : row) {
				
				switch (c) {
					case 'x':
						SDL_RenderCopy(renderer, pellet, nullptr, &mapRect);
						break;
					case '1' :
						SDL_RenderCopy(renderer, corner_bottom_left, nullptr, &mapRect);
						break;
					case '3':
						SDL_RenderCopy(renderer, corner_bottom_right, nullptr, &mapRect);
						break;
					case '7':
						SDL_RenderCopy(renderer, corner_top_left, nullptr, &mapRect);
						break;
					case '9':
						SDL_RenderCopy(renderer, corner_top_right, nullptr, &mapRect);
						break;
					case '2' :
						SDL_RenderCopy(renderer, wall_bottom, nullptr, &mapRect);
						break;
					case '4':
						SDL_RenderCopy(renderer, wall_left, nullptr, &mapRect);
						break;
					case '6':
						SDL_RenderCopy(renderer, wall_right, nullptr, &mapRect);
						break;
					case '8':
						SDL_RenderCopy(renderer, wall_top, nullptr, &mapRect);
						break;
					case ' ': 
					case '-':
						break;
				}
				mapRect.x += 16;
			}
			mapRect.x = 0;
			mapRect.y += 16;
		}

		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}

	//Program exit 
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return EXIT_SUCCESS;
}