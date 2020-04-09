//#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "GameManager.h"
#include "SDL_Manager.h"

//Window size
int SCREEN_WIDTH = 450;
int SCREEN_HEIGHT = 550;

void GameManager::setFramerate(const int FPS) {
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	frameStart = SDL_GetTicks();

	frameTime = SDL_GetTicks() - frameStart;

	if (frameDelay > frameTime) {
		SDL_Delay(frameDelay - frameTime);
	}
}

GameManager::GameManager() {}

int GameManager::play(std::string name) {

	auto sdl_manager = std::make_unique<SDL_Manager>();
	auto texture_manager = std::make_unique<Texture_Manager>();
	
	const int FPS = 60;

	std::vector<std::vector<char>> map{};
	loadMap(name, map);

	//prints map to console (FOR DEBUGGING)
	for (auto& row : map) {
		for (auto& c : row) {
			std::cout << c;
		}
		std::cout << std::endl;
	}

	SDL_Window *window = sdl_manager->createWindow("Pac-man", SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_Renderer *renderer = sdl_manager->createRenderer(window, -1);

	//draws background
	sdl_manager->SetRenderColor(renderer, 0, 0, 0, 255);
	sdl_manager->ClearRender(renderer);

	//create pacman
	SDL_Surface *surface = sdl_manager->createSurface("../images/pacman.png", window, renderer);
	SDL_Texture *drawable = texture_manager->draw(renderer, surface);

	auto p1 = std::make_unique<Player>(drawable);
	p1->setPos(0, 0);
	p1->setSize(16, 16);
 
	//Freeing the RGB surface
	SDL_FreeSurface(surface);

	//Create Textures
	SDL_Texture* pellet = texture_manager->loadTexture("../images/mapTiles/pellet.png", renderer);
	SDL_Texture* wall_bottom = texture_manager->loadTexture("../images/mapTiles/wall_bottom_single.png", renderer);
	SDL_Texture* wall_top = texture_manager->loadTexture("../images/mapTiles/wall_top_single.png", renderer);
	SDL_Texture* wall_left = texture_manager->loadTexture("../images/mapTiles/wall_left_single.png", renderer);
	SDL_Texture* wall_right = texture_manager->loadTexture("../images/mapTiles/wall_right_single.png", renderer);
	SDL_Texture* corner_top_right = texture_manager->loadTexture("../images/mapTiles/wall_corner_tr_single.png", renderer);
	SDL_Texture* corner_top_left = texture_manager->loadTexture("../images/mapTiles/wall_corner_tl_single.png", renderer);
	SDL_Texture* corner_bottom_right = texture_manager->loadTexture("../images/mapTiles/wall_corner_br_single.png", renderer);
	SDL_Texture* corner_bottom_left = texture_manager->loadTexture("../images/mapTiles/wall_corner_bl_single.png", renderer);

	//Det er n� 200 linjer (Deal or no deal with it)
	bool isRunning = true;
	const Uint8* keys = nullptr;
	int numKeys;
	keys = SDL_GetKeyboardState(&numKeys);
	SDL_Event evt;

	//movement variables
	char direction = 'i';

	//Wall vector
	//Use to check collison with walls
	std::vector<SDL_Rect> walls{};
	SDL_Rect mapRect;
	mapRect.w = 16;
	mapRect.h = 16;
	mapRect.x = 0;
	mapRect.y = 0;

	for (auto &row : map) {
		for (auto &c : row) {

			switch (c) {
				case '1':
					walls.emplace_back(mapRect);
					break;
				case '3':
					walls.emplace_back(mapRect);
					break;
				case '7':
					walls.emplace_back(mapRect);
					break;
				case '9':
					walls.emplace_back(mapRect);
					break;
				case '2':
					walls.emplace_back(mapRect);
					break;
				case '4':
					walls.emplace_back(mapRect);
					break;
				case '6':
					walls.emplace_back(mapRect);
					break;
				case '8':
					walls.emplace_back(mapRect);
					break;
				default:
					break;
			}
			mapRect.x += 16;
		}
		mapRect.x = 0;
		mapRect.y += 16;
	}

	//Debug
	for (size_t i = 0; i < walls.size(); i++) {
		std::cout << walls[i].x << ", " << walls[i].y << std::endl;
	}

	//Game Loop
	while (isRunning) {

		setFramerate(FPS);

		//Checks if Escape is press or X in the window
		if (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_KEYDOWN) {
				if (evt.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false;
				}
			}
			else if (evt.type == SDL_QUIT) {
				isRunning = false;
			}
		}

		//Keys input for movement
		p1->movePlayer(keys, surface, SCREEN_WIDTH, SCREEN_HEIGHT);

		//Prepare Renderer for a new frame
		SDL_RenderCopy(renderer, p1->getTexture(), nullptr, p1->getCoords());
		
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
				case '1':
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
				case '2':
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
				case 'S':
				case 's':
					p1->setPos(mapRect.x, mapRect.y);
					c = ' ';
					break;
				}
				mapRect.x += 16;
			}
			mapRect.x = 0;
			mapRect.y += 16;
		}
		p1->checkWallCollision(walls);

		/*
		bool collided = p1->checkWallCollision(walls);
	
		if (collided) {
			//std::cout << "Wall hit" << std::endl;
			p1->setDirection('i');
		} else {
			//std::cout << "Didnt hit a wall" << std::endl;
		}

		//std::cout << "x: " << p1->getCoords()->x << ", Y: " << p1->getCoords()->y << std::endl;
		*/

		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}

	//Program exit 
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return EXIT_SUCCESS;
}

void GameManager::loadMap(std::string map, std::vector<std::vector<char>> &mapVector) {
	std::ifstream fIn(map);

	if (!fIn.is_open()) {
		std::cout << "Cannot open map file" << std::endl;
		return;
	}

	std::string str;

	while (std::getline(fIn, str)) {

		std::vector<char> mapRow;

		for (size_t i = 0; i < str.length(); i++) {
			mapRow.emplace_back(str[i]);
		}

		mapVector.emplace_back(mapRow);
	}

	fIn.close();
}

void GameManager::drawMap(std::vector<std::vector<char>> &mapVector) {}