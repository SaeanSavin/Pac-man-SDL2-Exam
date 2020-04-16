#include <fstream>
#include <sstream>
//#include <string>

#include "GameManager.h"
#include "SDL_Manager.h"

//Window size
int SCREEN_WIDTH{};
int SCREEN_HEIGHT{};
int GAME_OFFSET_Y{};
int GAME_OFFSET_X{};

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

enum class TargetType {
	AGRESSIVE,
	SUPPORTIVE,
	AMBUSH,
	EVASIVE,
};

int GameManager::play(std::string name) {

	//input
	const Uint8* keys = nullptr;
	int numKeys;
	keys = SDL_GetKeyboardState(&numKeys);
	SDL_Event evt;

	const int FPS = 60;

	/*   MAP SETUP   */

	std::cout << "loading map..." << std::endl;

	std::vector<std::vector<char>> map{};
	loadMap(name, map);

	GAME_OFFSET_Y = 100;
	GAME_OFFSET_X = 0;

	SCREEN_HEIGHT = map.size() * 16 + GAME_OFFSET_Y;
	SCREEN_WIDTH = map[0].size() * 16 + GAME_OFFSET_X;


	/*   SDL SETUP   */

	std::cout << "initializing SDL..." << std::endl;

	SDL_Init(SDL_INIT_AUDIO);

	auto sdl_manager = std::make_unique<SDL_Manager>();
	auto texture_manager = std::make_unique<Texture_Manager>();
	
	SDL_Window* window = sdl_manager->createWindow("Pac-man", SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_Renderer* renderer = sdl_manager->createRenderer(window, -1);

	//draws background
	sdl_manager->SetRenderColor(renderer, 0, 0, 0, 255);
	sdl_manager->ClearRender(renderer);


	/*   HUD SETUP   */

	std::cout << "building HUD..." << std::endl;

	//Text 
	SDL_Surface *textSurface = sdl_manager->createSurface("../images/TextTiles/text.png", window, renderer);
	SDL_Texture *text = texture_manager->draw(renderer, textSurface);

	//HP 
	SDL_Surface *hpSurface = sdl_manager->createSurface("../images/Pacman/move/2.png", window, renderer);
	SDL_Texture *hpTexture = texture_manager->draw(renderer, hpSurface);

	/*    VARIABLES   */

	bool isRunning = true;

	//Wall vector
	//Use to check collison with walls
	std::vector<SDL_Rect> walls{};

	//pellet vector
	//Use to check collision on pellets
	std::vector<SDL_Rect> pellets{};

	//walkable vector
	std::vector<SDL_Rect> walkable{};

	SDL_Rect mapRect = sdl_manager->createRect(16, 16, 0, 50);

	for (auto& row : map) {
		for (auto& c : row) {

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
			case '-':
				walls.emplace_back(mapRect);
				break;
			case '~':
				walkable.emplace_back(mapRect);
				break;
			case 'x':
				pellets.emplace_back(mapRect);
				walkable.emplace_back(mapRect);
				break;
			case ' ':
				walkable.emplace_back(mapRect);
				break;
			case 'G': case 'H': case 'O': case 'S': case 'P': case 'C':
				walkable.emplace_back(mapRect);
				break;
			default:
				break;
			}
			mapRect.x += 16;
		}
		mapRect.x = 0;
		mapRect.y += 16;
	}

	/* CHARACTER SETUP  */

	std::cout << "building characters..." << std::endl;

	//pacman
	SDL_Surface *surface = sdl_manager->createSurface("../images/pacman/move/1.png", window, renderer);
	SDL_Texture *pac_texture = texture_manager->draw(renderer, surface);

	std::shared_ptr<Character> p1 = std::make_shared<Player>(pac_texture, renderer, keys, pellets);
	p1->setPos(0, 0);
	p1->setSize(16, 16);

	auto pacman_move = std::make_shared<Animation>(renderer, "../images/Pacman/move", 12);
	p1->setAnimation("move", pacman_move);

	//shadow
	SDL_Texture *shadow_texture = texture_manager->loadTexture("../images/Ghosts/Shadow/shadow.png", renderer);

	auto shadow = std::make_shared<Ghost>(shadow_texture, renderer, walkable);
fixed	shadow->setPos(32, 0);
	shadow->setSize(16, 16);

	auto shadow_up = std::make_shared<Animation>(renderer, "../images/Ghosts/Shadow/move/up", 12);
	auto shadow_down = std::make_shared<Animation>(renderer, "../images/Ghosts/Shadow/move/down", 12);
	auto shadow_left = std::make_shared<Animation>(renderer, "../images/Ghosts/Shadow/move/left", 12);
	auto shadow_right = std::make_shared<Animation>(renderer, "../images/Ghosts/Shadow/move/right", 12);

	shadow->setAnimation("up", shadow_up);
	shadow->setAnimation("down", shadow_down);
	shadow->setAnimation("left", shadow_left);
	shadow->setAnimation("right", shadow_right);

	//clyde
	SDL_Texture* pokey_texture = texture_manager->loadTexture("../images/Ghosts/Pokey/pokey.png", renderer);

	auto pokey = std::make_shared<Ghost>(pokey_texture, renderer, walkable);
	pokey->setPos(32, 0);
	pokey->setSize(16, 16);

	auto pokey_up = std::make_shared<Animation>(renderer, "../images/Ghosts/Pokey/move/up", 12);
	auto pokey_down = std::make_shared<Animation>(renderer, "../images/Ghosts/Pokey/move/down", 12);
	auto pokey_left = std::make_shared<Animation>(renderer, "../images/Ghosts/Pokey/move/left", 12);
	auto pokey_right = std::make_shared<Animation>(renderer, "../images/Ghosts/Pokey/move/right", 12);

	pokey->setAnimation("up", pokey_up);
	pokey->setAnimation("down", pokey_down);
	pokey->setAnimation("left", pokey_left);
	pokey->setAnimation("right", pokey_right);

	//Freeing the RGB surface
	SDL_FreeSurface(surface);
	SDL_FreeSurface(textSurface);


	/*   TEXTURE SETUP   */
	
	std::cout << "loading assets..." << std::endl;

	//Create Textures
	SDL_Texture *pellet = texture_manager->loadTexture("../images/mapTiles/pellet.png", renderer);
	SDL_Texture *cherry = texture_manager->loadTexture("../images/mapTiles/cherry.png", renderer);
	SDL_Texture *wall_bottom = texture_manager->loadTexture("../images/mapTiles/wall_bottom_single.png", renderer);
	SDL_Texture *wall_top = texture_manager->loadTexture("../images/mapTiles/wall_top_single.png", renderer);
	SDL_Texture *wall_left = texture_manager->loadTexture("../images/mapTiles/wall_left_single.png", renderer);
	SDL_Texture *wall_right = texture_manager->loadTexture("../images/mapTiles/wall_right_single.png", renderer);
	SDL_Texture *corner_top_right = texture_manager->loadTexture("../images/mapTiles/wall_corner_tr_single.png", renderer);
	SDL_Texture *corner_top_left = texture_manager->loadTexture("../images/mapTiles/wall_corner_tl_single.png", renderer);
	SDL_Texture *corner_bottom_right = texture_manager->loadTexture("../images/mapTiles/wall_corner_br_single.png", renderer);
	SDL_Texture *corner_bottom_left = texture_manager->loadTexture("../images/mapTiles/wall_corner_bl_single.png", renderer);

	/*   AUDIO SETUP   */

	std::cout << "preparing audio..." << std::endl;

	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8* wavBuffer;

	SDL_LoadWAV("../audio/intro.wav", &wavSpec, &wavBuffer, &wavLength);

	SDL_AudioDeviceID deviceID = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

	int success = SDL_QueueAudio(deviceID, wavBuffer, wavLength);
	//SDL_PauseAudioDevice(deviceID, 0);

	//SDL_Delay(8000);


	/*   GAME LOOP START  */

	while (isRunning) {

		setFramerate(FPS);

		//Close on ESC
		if (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_KEYDOWN) {
				if (evt.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false;
				}
			}
		}
		
		//Print score
		SDL_Rect text_src = sdl_manager->createRect(8, 8, 0, 0);
		SDL_Rect text_dst = sdl_manager->createRect(16,16, 0, 25);

		texture_manager->printFromTiles("SCORE ", renderer, text, text_dst, text_src);
		texture_manager->printPlayerScore(p1->getScore(), renderer, text, text_dst, text_src);

		SDL_Rect hp_dst = sdl_manager->createRect(16, 16, 0, SCREEN_HEIGHT - 25);
		texture_manager->printFromTiles("LIVES ", renderer, text, hp_dst, text_src);
		
		int pCoordsLeft = p1->getCoords()->x;
		int pCoordsRight = p1->getCoords()->x + p1->getCoords()->w;
		int pCoordsUp = p1->getCoords()->y;
		int pCoordsDown = p1->getCoords()->y + p1->getCoords()->h;

		int gCoordsLeft = pokey->getCoords()->x;
		int gCoordsRight = pokey->getCoords()->x + pokey->getCoords()->w;
		int gCoordsUp = pokey->getCoords()->y;
		int gCoordsDown = pokey->getCoords()->y + pokey->getCoords()->h;

		if (pCoordsLeft < gCoordsRight && pCoordsRight > gCoordsLeft) {
			if (pCoordsUp < gCoordsDown && pCoordsDown > gCoordsUp) {
				p1->hitByGhost();
				SDL_Delay(1000);
				if (p1->getHP() <= 0) {
					isRunning = false;
				}
			}
		}

		for (int i = 0; i < p1->getHP(); i++) {
			SDL_RenderCopy(renderer, hpTexture, nullptr, &hp_dst);
			hp_dst.x += 20;
		}

		//Move characters
		p1->move(surface, SCREEN_WIDTH, SCREEN_HEIGHT, map, walls);

		shadow->setTarget(getTarget(TargetType::AGRESSIVE, p1));
		shadow->move(surface, SCREEN_WIDTH, SCREEN_HEIGHT, map, walls);

		pokey->setTarget(getTarget(TargetType::AGRESSIVE, p1));
		pokey->move(surface, SCREEN_WIDTH, SCREEN_HEIGHT, map, walls);

		//render map
		SDL_Rect mapRect = sdl_manager->createRect(16, 16, 0, 50);

		for (auto& row : map) {
			for (auto& c : row) {

				switch (c) {
				case 'x':
					SDL_RenderCopy(renderer, pellet, nullptr, &mapRect);
					break;
				case 'C':
					SDL_RenderCopy(renderer, cherry, nullptr, &mapRect);
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
					p1->setPos(mapRect.x, mapRect.y);
					p1->setSpawnPos(mapRect.x, mapRect.y);
					c = ' ';
					break;
				case 'G':
					shadow->setPos(mapRect.x, mapRect.y);
					c = ' ';
					break;
				case 'H':
					pokey->setPos(mapRect.x, mapRect.y);
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

	/*   GAME LOOP END   */

	//Program exit 
	SDL_CloseAudioDevice(deviceID);

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

std::pair<int, int> GameManager::getTarget(TargetType mode, std::shared_ptr<Character> enemy) {
	std::pair<int, int> target;
	switch (mode) {
		case TargetType::AGRESSIVE:
			//target pacman directly
			target.first = enemy->getCoords()->x;
			target.second = enemy->getCoords()->y;
			return target;
	}
}