#include <fstream>
#include <sstream>
#include <string>

#include "GameManager.h"
#include "SDL_Manager.h"

//Window size
int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;

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
	SCREEN_HEIGHT = map.size() * 16 + 100;
	SCREEN_WIDTH = map[0].size() * 16;

	SDL_Window *window = sdl_manager->createWindow("Pac-man", SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_Renderer *renderer = sdl_manager->createRenderer(window, -1);

	//draws background
	sdl_manager->SetRenderColor(renderer, 0, 0, 0, 255);
	sdl_manager->ClearRender(renderer);

	//create pacman
	SDL_Surface *surface = sdl_manager->createSurface("../images/pacman/move/1.png", window, renderer);
	SDL_Texture *player = texture_manager->draw(renderer, surface);

	//Text 
	SDL_Surface *textSurface = sdl_manager->createSurface("../images/text.png", window, renderer);
	SDL_Texture *text = texture_manager->draw(renderer, textSurface);

	auto p1 = std::make_unique<Player>(player, renderer);
	p1->setPos(0, 0);
	p1->setSize(16, 16);

	//create pacman animations
	auto pacman_move = std::make_shared<Animation>(renderer, "../images/Pacman/move", 12);
	p1->setMoveAnimation(pacman_move);
 
	//Freeing the RGB surface
	SDL_FreeSurface(surface);
	SDL_FreeSurface(textSurface);

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

	//pellet vector
	//Use to check collision on pellets
	std::vector<SDL_Rect> pellets{};

	mapRect.w = 16;
	mapRect.h = 16;
	mapRect.x = 0;
	mapRect.y = 50;

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
				case 'x':
					pellets.emplace_back(mapRect);
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
		
		//Prints out 8x8 from text.png
		//S
		SDL_Rect dest;
		dest.w = 8;
		dest.h = 8;
		dest.x = 24;
		dest.y = 8;

		SDL_Rect textRect;
		textRect.w = 16;
		textRect.h = 16;
		textRect.x = 0;
		textRect.y = 0;

		//C
		SDL_RenderCopy(renderer, text, &dest, &textRect);
		textRect.x += 16;
		dest.x = 16;
		dest.y = 0;

		//O
		SDL_RenderCopy(renderer, text, &dest, &textRect);
		textRect.x += 16;
		dest.x = 112;
		dest.y = 0;
		
		//R
		SDL_RenderCopy(renderer, text, &dest, &textRect);
		textRect.x += 16;
		dest.x = 16;
		dest.y = 8;

		//E
		SDL_RenderCopy(renderer, text, &dest, &textRect);
		textRect.x += 16;
		dest.x = 32;
		dest.y = 0;

		//Space
		SDL_RenderCopy(renderer, text, &dest, &textRect);
		textRect.x += 32;
		dest.x = 0;
		dest.y = 16;
		
		// 0
		SDL_RenderCopy(renderer, text, &dest, &textRect);

		//Keys input for movement
		p1->movePlayer(keys, surface, SCREEN_WIDTH, SCREEN_HEIGHT, map, walls, pellets);
		//std::cout << p1->getScore() << std::endl;

		//Prepare Renderer for a new frame
		//SDL_RenderCopy(renderer, p1->getTexture(), nullptr, p1->getCoords());
		
		//render map
		SDL_Rect mapRect;
		mapRect.w = 16;
		mapRect.h = 16;
		mapRect.x = 0;
		mapRect.y = 50;

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