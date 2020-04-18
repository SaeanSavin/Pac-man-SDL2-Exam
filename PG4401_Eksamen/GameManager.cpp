#include <fstream>
#include <sstream>

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

void GameManager::play(std::string name) {

	//input
	const Uint8* keys = nullptr;
	int numKeys;
	keys = SDL_GetKeyboardState(&numKeys);
	SDL_Event evt;

	SDL_GameController *gameController = nullptr;

	std::string startText = "READY!";
	std::string gameoverText = "GAME OVER!";
	std::string levelcompletedText = "LEVEL COMPLETED!";

	const int FPS = 60;

	/*   MAP SETUP   */

	std::cout << "loading map..." << std::endl;

	std::vector<std::vector<char>> map{};
	loadMap(name, map);

	std::vector<std::vector<char>> defaultMap = map;

	GAME_OFFSET_Y = 100;
	GAME_OFFSET_X = 0;

	SCREEN_HEIGHT = map.size() * 16 + GAME_OFFSET_Y;
	SCREEN_WIDTH = map[0].size() * 16 + GAME_OFFSET_X;


	/*   SDL SETUP   */

	std::cout << "initializing SDL..." << std::endl;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);

	auto sdl_manager = std::make_unique<SDL_Manager>();
	auto texture_manager = std::make_shared<Texture_Manager>();
	
	SDL_Window* window = sdl_manager->createWindow("Pac-man", SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_Renderer* renderer = sdl_manager->createRenderer(window, -1);

	//draws background
	sdl_manager->SetRenderColor(renderer, 0, 0, 0, 255);
	sdl_manager->clearRenderer(renderer);


	/*   HUD SETUP   */

	std::cout << "building HUD..." << std::endl;

	//Text 
	SDL_Surface *textSurface = sdl_manager->createSurface("../images/TextTiles/text.png", window, renderer);
	SDL_Texture *text = texture_manager->draw(renderer, textSurface);

	//HP 
	SDL_Surface *hpSurface = sdl_manager->createSurface("../images/Pacman/move/2.png", window, renderer);
	SDL_Texture *hpTexture = texture_manager->draw(renderer, hpSurface);

	/*    VECTORS   */

	//Wall vector
	//Use to check collison with walls
	std::vector<SDL_Rect> walls{};

	//pellet vector
	//Use to check collision on edible
	std::vector<SDL_Rect> edible{};
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
				edible.emplace_back(mapRect);
				walkable.emplace_back(mapRect);
				break;
			case 'C':
				edible.emplace_back(mapRect);
				walkable.emplace_back(mapRect);
				break;
			case 'P':
				edible.emplace_back(mapRect);
				walkable.emplace_back(mapRect);
			case ' ':
				walkable.emplace_back(mapRect);
				break;
			case 'G':
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
	gameController = sdl_manager->getGameController();

	std::shared_ptr<Player> p1 = std::make_shared<Player>(pac_texture, renderer, keys, edible, gameController);
	p1->setPos(0, 0);
	p1->setSize(16, 16);

	auto pacman_move = std::make_shared<Animation>(renderer, "../images/Pacman/move", 12);
	p1->setAnimation("move", pacman_move);

	auto pacman_dead = std::make_shared<Animation>(renderer, "../images/Pacman/dead", 120);
	p1->setAnimation("dead", pacman_dead);

	//build ghosts and set spawn positions
	std::vector<std::shared_ptr<Ghost>> ghosts;
	int ghostNr = 1;
	int g_index = 0;
	SDL_Rect m_position = sdl_manager->createRect(16, 16, 0, 50);

	for (auto& row : map) {
		for (auto& c : row) {
			switch (c) {
				case 'G':
					switch (ghostNr) {
						case 1:
							ghosts.emplace_back(makeGhost(texture_manager, renderer, walkable, GhostType::SHADOW));
							ghosts[g_index]->setSpawnPos(m_position.x, m_position.y);
							ghostNr++;
							g_index++;
							break;
						case 2:
							ghosts.emplace_back(makeGhost(texture_manager, renderer, walkable, GhostType::SPEEDY));
							ghosts[g_index]->setSpawnPos(m_position.x, m_position.y);
							ghostNr++;
							g_index++;
							break;
						case 3:
							ghosts.emplace_back(makeGhost(texture_manager, renderer, walkable, GhostType::BASHFUL));
							ghosts[g_index]->setSpawnPos(m_position.x, m_position.y);
							ghostNr++;
							g_index++;
							break;
						case 4:
							ghosts.emplace_back(makeGhost(texture_manager, renderer, walkable, GhostType::POKEY));
							ghosts[g_index]->setSpawnPos(m_position.x, m_position.y);
							ghostNr++;
							g_index++;
							break;
					}
					c = '~';
					break;
				case 'S':
					p1->setSpawnPos(m_position.x, m_position.y);
					break;
			}
			m_position.x += 16;
		}
		m_position.x = 0;
		m_position.y += 16;
	}

	//Freeing the RGB surface
	SDL_FreeSurface(surface);
	SDL_FreeSurface(textSurface);


	/*   TEXTURE SETUP   */
	
	std::cout << "loading assets..." << std::endl;

	//Create Textures
	SDL_Texture *pellet = texture_manager->loadTexture("../images/Edible/pellet.png", renderer);
	SDL_Texture *cherry = texture_manager->loadTexture("../images/Edible/cherry.png", renderer);
	SDL_Texture *poweups = texture_manager->loadTexture("../images/Edible/powerup.png", renderer);
	SDL_Texture *wall_bottom = texture_manager->loadTexture("../images/mapTiles/wall_bottom_single.png", renderer);
	SDL_Texture *wall_top = texture_manager->loadTexture("../images/mapTiles/wall_top_single.png", renderer);
	SDL_Texture *wall_left = texture_manager->loadTexture("../images/mapTiles/wall_left_single.png", renderer);
	SDL_Texture *wall_right = texture_manager->loadTexture("../images/mapTiles/wall_right_single.png", renderer);
	SDL_Texture *corner_top_right = texture_manager->loadTexture("../images/mapTiles/wall_corner_tr_single.png", renderer);
	SDL_Texture *corner_top_left = texture_manager->loadTexture("../images/mapTiles/wall_corner_tl_single.png", renderer);
	SDL_Texture *corner_bottom_right = texture_manager->loadTexture("../images/mapTiles/wall_corner_br_single.png", renderer);
	SDL_Texture *corner_bottom_left = texture_manager->loadTexture("../images/mapTiles/wall_corner_bl_single.png", renderer);

	SDL_Rect text_src = sdl_manager->createRect(8, 8, 0, 0);


	/*   AUDIO SETUP   */

	std::cout << "preparing audio..." << std::endl;

	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8* wavBuffer;

	SDL_LoadWAV("../audio/intro.wav", &wavSpec, &wavBuffer, &wavLength);

	SDL_AudioDeviceID deviceID = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

	int success = SDL_QueueAudio(deviceID, wavBuffer, wavLength);
	SDL_PauseAudioDevice(deviceID, 0);

	SDL_Rect readyDst = sdl_manager->createRect(16, 16, SCREEN_WIDTH / 2 - (startText.length() * 8), SCREEN_HEIGHT / 2);
	texture_manager->printFromTiles(startText, renderer, text, readyDst, text_src);
	sdl_manager->clearAndUpdateRenderer(renderer);
	
	SDL_Delay(8000);

	/*    VARIABLES   */

	//check if game is running
	bool isRunning = true;
	
	//check and manage powered-state of pacman
	bool isPowered = false;
	Uint32 poweredTime = 1000000;
	Uint32 poweredStart = 0;
	Uint32 poweredDelta = 0;

	//time variables for chase-scatter cycle
	Uint32 roundStart = SDL_GetPerformanceCounter();
	Uint32 scatterDuration = 7;
	Uint32 chaseDuration = 20;
	Uint32 cycle = 0;
	Uint32 cycleDelta = 0;
	Uint32 scatterCount = 0;
	Uint32 currentLevel = 0;
	bool chasing = false;


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

		SDL_Rect text_dst = sdl_manager->createRect(16,16, 0, 25);

		texture_manager->printFromTiles("SCORE ", renderer, text, text_dst, text_src);
		texture_manager->printPlayerScore(p1->getScore(), renderer, text, text_dst, text_src);

		SDL_Rect hp_dst = sdl_manager->createRect(16, 16, 0, SCREEN_HEIGHT - 25);
		texture_manager->printFromTiles("LIVES ", renderer, text, hp_dst, text_src);
		
		//collision
		int pCoordsLeft = p1->getCoords()->x;
		int pCoordsRight = p1->getCoords()->x + p1->getCoords()->w;
		int pCoordsUp = p1->getCoords()->y;
		int pCoordsDown = p1->getCoords()->y + p1->getCoords()->h;

		for (auto &g : ghosts) {
			int gCoordsLeft = g->getCoords()->x;
			int gCoordsRight = g->getCoords()->x + g->getCoords()->w;
			int gCoordsUp = g->getCoords()->y;
			int gCoordsDown = g->getCoords()->y + g->getCoords()->h;

			if (pCoordsLeft < gCoordsRight && pCoordsRight > gCoordsLeft) {
				if (pCoordsUp < gCoordsDown && pCoordsDown > gCoordsUp) {
					//if pacman hits ghost while 
					if (g->isFrightened()) {
						g->hitByPacman();
						SDL_Delay(1000);
						p1->addScore(1000);
					} else if (!g->isEaten()) {
						p1->hitByGhost();
						isPowered = false;
						poweredStart = 0;
						for (auto& g : ghosts) {
							g->respawn();
						}
						if (p1->getHP() <= 0) {
							isRunning = false;
						}
					}
				}
			}
		}

		for (int i = 0; i < p1->getHP(); i++) {
			SDL_RenderCopy(renderer, hpTexture, nullptr, &hp_dst);
			hp_dst.x += 20;
		}

		//manage chase-scatter cycle

		cycleDelta = (SDL_GetPerformanceCounter() - cycle) * 1000 / SDL_GetPerformanceCounter();
		cycle += cycleDelta;

		if(!chasing && cycle >= scatterDuration * 50000) {
			cycle = 0;
			chasing = true;
		} 
		if (chasing && cycle >= chaseDuration * 50000) {
			cycle = 0;
			chasing = false;
		}

		//Move characters and check if pacman is powered
		p1->move(surface, SCREEN_WIDTH, SCREEN_HEIGHT, map, walls);

		//runs on the frame that pacman gets power pellet
		if (p1->isPowered()) {
			p1->stopPowered();
			isPowered = true;
			poweredStart = 0;
			poweredDelta = SDL_GetPerformanceCounter();
			for (auto& g : ghosts) {
				if (!g->isEaten()) {
					g->stopFrightenedEnding();
					g->startFrightened();
				}
			}
		}

		//runs while pacman is powered
		if (isPowered) {

			poweredDelta = (SDL_GetPerformanceCounter() - poweredStart) * 1000 / SDL_GetPerformanceCounter();
			poweredStart += poweredDelta;

			//check if frightened is over in 25% of time
			if ((poweredStart * 100) / poweredTime >= 75) {
				for (auto& g : ghosts) {
					if (g->isFrightened()) {
						g->startFrightenedEnding();
					}
				}
			}

			//check if frightened over;
			if (poweredStart >= poweredTime) {
				isPowered = false;
				for (auto& g : ghosts) {
					if (g->isFrightened()) {
						g->stopFrightened();
					}
				}
				poweredStart = 0;
			}
		}

		//decide target type for ghosts and move ghosts accordingly
		int ghosts_i = 0;
		for (auto& g : ghosts) {
			//if ghost is eaten, return to spawn
			if (g->isEaten()) {
				g->setTarget(getTarget(TargetType::RETURN, g));
			}
			//if pacman is powered, ghosts should be frightened
			else if (isPowered) {
				g->setTarget(getTarget(TargetType::FRIGHTENED));
			}
			//if not chasing, scatter
			else if (!chasing) {
				g->setTarget(getTarget(TargetType::SCATTER, g));
			}
			//else, chase based on TargetMode pattern
			else {
				if (g->getTargetMode() == TargetType::EVASIVE) {
					g->setTarget(getTarget(g->getTargetMode(), p1, g));
				}
				else if (g->getTargetMode() == TargetType::SUPPORTIVE) {
					g->setTarget(getTarget(g->getTargetMode(), p1, ghosts[ghosts_i - 2]));
				}
				else {
					g->setTarget(getTarget(g->getTargetMode(), p1));
				}
			}
			g->move(surface, SCREEN_WIDTH, SCREEN_HEIGHT, map, walls);
			ghosts_i++;
		}

		//render map
		SDL_Rect mapRect = sdl_manager->createRect(16, 16, 0, 50);

		int currentPellets = 0;

		for (auto& row : map) {
			for (auto& c : row) {

				switch (c) {
				case 'x':
					SDL_RenderCopy(renderer, pellet, nullptr, &mapRect);
					currentPellets++;
					break;
				case 'C':
					SDL_RenderCopy(renderer, cherry, nullptr, &mapRect);
					break;
				case 'P':
					SDL_RenderCopy(renderer, poweups, nullptr, &mapRect);
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
					break;
				case 'G':
					break;
				}
				mapRect.x += 16;
			}
			mapRect.x = 0;
			mapRect.y += 16;
		}

		sdl_manager->clearAndUpdateRenderer(renderer);

		if (p1->getHP() <= 0) {
			setTotalPlayerScore(p1->getScore());
			SDL_Rect gameoverDst = sdl_manager->createRect(16, 16, SCREEN_WIDTH / 2 - (gameoverText.length() * 8), SCREEN_HEIGHT / 2);
			texture_manager->printFromTiles(gameoverText, renderer, text, gameoverDst, text_src);
			sdl_manager->clearAndUpdateRenderer(renderer);
			isRunning = false;
			SDL_Delay(2000);
		}

		if (currentPellets <= 0 && p1->getHP() > 0) {
			SDL_Rect levelcompletedDst = sdl_manager->createRect(16, 16, SCREEN_WIDTH / 2 - (levelcompletedText.length() * 8), SCREEN_HEIGHT / 2);
			texture_manager->printFromTiles(levelcompletedText, renderer, text, levelcompletedDst, text_src);
			sdl_manager->clearAndUpdateRenderer(renderer);
			p1->respawn();
			for (auto &g : ghosts) {
				g->respawn();
			}
			map = defaultMap;
			SDL_Delay(2000);
		}
	}

	/*   GAME LOOP END   */

	//Program exit 
	SDL_CloseAudioDevice(deviceID);
	SDL_GameControllerClose(0);

	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return;
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

void GameManager::setTotalPlayerScore(int playerScore) {
	totalScore += playerScore;
}

std::shared_ptr<Ghost> GameManager::makeGhost(std::shared_ptr<Texture_Manager> texture_manager, SDL_Renderer* renderer, std::vector<SDL_Rect>& walkable, GhostType type) {
	std::string ghost_path = "../images/Ghosts/";
	std::string ghost_anim_path = "../images/Ghosts/";

	TargetType mode;

	switch (type) {
		case GhostType::SHADOW:
			ghost_path += "Shadow/shadow.png";
			ghost_anim_path += "Shadow/";
			mode = TargetType::AGRESSIVE;
			break;
		case GhostType::SPEEDY:
			ghost_path += "Speedy/speedy.png";
			ghost_anim_path += "Speedy/";
			mode = TargetType::AMBUSH;
			break;
		case GhostType::BASHFUL:
			ghost_path += "Bashful/bashful.png";
			ghost_anim_path += "Bashful/";
			mode = TargetType::SUPPORTIVE;
			break;
		case GhostType::POKEY:
			ghost_path += "Pokey/pokey.png";
			ghost_anim_path += "Pokey/";
			mode = TargetType::EVASIVE;
			break;
	}

	SDL_Texture* ghost_texture = texture_manager->loadTexture(&ghost_path[0], renderer);

	auto ghost = std::make_shared<Ghost>(ghost_texture, renderer, walkable, mode, type);
	ghost->setPos(32, 0);
	ghost->setSize(16, 16);

	auto ghost_up = std::make_shared<Animation>(renderer, (ghost_anim_path + "move/up"), 12);
	auto ghost_down = std::make_shared<Animation>(renderer, (ghost_anim_path + "move/down"), 12);
	auto ghost_left = std::make_shared<Animation>(renderer, (ghost_anim_path + "move/left"), 12);
	auto ghost_right = std::make_shared<Animation>(renderer, (ghost_anim_path + "move/right"), 12);

	auto frightened = std::make_shared<Animation>(renderer, "../images/Ghosts/Frightened/move", 12);
	auto frightened_ending = std::make_shared<Animation>(renderer, "../images/Ghosts/Frightened/move_last", 12);

	auto eyes_up = std::make_shared<Animation>(renderer, "../images/Ghosts/Eyes/move/up", 12);
	auto eyes_down = std::make_shared<Animation>(renderer, "../images/Ghosts/Eyes/move/down", 12);
	auto eyes_left = std::make_shared<Animation>(renderer, "../images/Ghosts/Eyes/move/left", 12);
	auto eyes_right = std::make_shared<Animation>(renderer, "../images/Ghosts/Eyes/move/right", 12);

	ghost->setAnimation("up", ghost_up);
	ghost->setAnimation("down", ghost_down);
	ghost->setAnimation("left", ghost_left);
	ghost->setAnimation("right", ghost_right);
	
	ghost->setAnimation("frightened", frightened);
	ghost->setAnimation("frightened_ending", frightened_ending);

	ghost->setAnimation("eaten_up", eyes_up);
	ghost->setAnimation("eaten_down", eyes_down);
	ghost->setAnimation("eaten_left", eyes_left);
	ghost->setAnimation("eaten_right", eyes_right);

	return ghost;
}

std::pair<int, int> GameManager::getTarget(TargetType mode) {
	std::pair<int, int> target;
	switch (mode) {
	case TargetType::FRIGHTENED:
		//randomly choose target
		int random = rand() % 4 + 1;
		if (random == 4) {
			target.first = 0;
			target.second = 0;
		}
		else if (random == 3) {
			target.first = SCREEN_WIDTH;
			target.second = 0;
		}
		else if (random == 2) {
			target.first = 0;
			target.second = SCREEN_HEIGHT;
		}
		else {
			target.first = SCREEN_WIDTH;
			target.second = SCREEN_HEIGHT;
		}
		break;
	}
	return target;
}

//overloaded version of getTarget for AGRESSIVE and AMBUSH targetting behaviour
std::pair<int, int> GameManager::getTarget(TargetType mode, std::shared_ptr<Player> enemy) {
	std::pair<int, int> target;
	switch (mode) {
	case TargetType::AGRESSIVE:
		//target enemy directly
		target.first = enemy->getCoords()->x;
		target.second = enemy->getCoords()->y;
		return target;
	case TargetType::AMBUSH:
		//target 4x16 ahead of enemys current direction
		target.first = enemy->getCoords()->x;
		target.second = enemy->getCoords()->y;
		switch (enemy->getDirection()) {
		case 'w':
			target.second -= 4 * 16;
			break;
		case 's':
			target.second += 4 * 16;
			break;
		case 'a':
			target.first -= 4 * 16;
			break;
		case 'd':
			target.first += 4 * 16;
			break;
		case ' ':
			target.first = enemy->getCoords()->x;
			target.second = enemy->getCoords()->y;
			break;
		}
		return target;
	}
	return target;
}

//overloaded version of getTarget for SUPPORTIVE and EVASIVE targetting behaviour
std::pair<int, int> GameManager::getTarget(TargetType mode, std::shared_ptr<Player> enemy, std::shared_ptr<Ghost> ghost) {
	std::pair<int, int> target;
	int v_x = 0;
	int v_y = 0;
	switch (mode) {
		case TargetType::SUPPORTIVE:
			//targeted tile is based on another ghosts position (originally blinkys)
			target.first = enemy->getCoords()->x;
			target.second = enemy->getCoords()->y;

			//first, find tile 2x16 pixels ahead of pacmans current direction
			switch (enemy->getDirection()) {
			case 'w':
				target.second -= 2 * 16;
				break;
			case 's':
				target.second += 2 * 16;
				break;
			case 'a':
				target.first -= 2 * 16;
				break;
			case 'd':
				target.first += 2 * 16;
				break;
			case ' ':
				target.first = enemy->getCoords()->x;
				target.second = enemy->getCoords()->y;
				return target;
			}

			//then, calculate vector from this position to ghost ally
			v_x = abs(ghost->getCoords()->x - target.first);
			v_y = abs(ghost->getCoords()->y - target.second);

			//finally, rotate vector 180 degrees and set target to this position
			target.first += v_x * -1;
			target.second += v_y * -1;
			return target;

		case TargetType::EVASIVE:
			//target using AGRESSIVE if 16*8 away from enemy, else, scatter
			int distance = std::sqrt(std::pow(abs(ghost->getCoords()->x - enemy->getCoords()->x), 2) + std::pow(abs(ghost->getCoords()->y - enemy->getCoords()->y), 2));
			if (distance >= 8 * 16) {
				return getTarget(TargetType::AGRESSIVE, enemy);
			}
			else {
				return getTarget(TargetType::SCATTER, ghost);
			}
	}
}

//overloaded version of getTarget for SCATTER and RETURN
std::pair<int, int> GameManager::getTarget(TargetType mode, std::shared_ptr<Ghost> ghost) {
	std::pair<int, int> target;
	switch (mode) {
		//target one of four map corners, based on which type of ghost they are
	case TargetType::SCATTER:
		switch (ghost->getType()) {
		case GhostType::SHADOW:
			target.first = SCREEN_WIDTH;
			target.second = 0;
			break;
		case GhostType::SPEEDY:
			target.first = 0;
			target.second = 0;
			break;
		case GhostType::BASHFUL:
			target.first = SCREEN_WIDTH;
			target.second = SCREEN_HEIGHT;
			break;
		case GhostType::POKEY:
			target.first = 0;
			target.second = SCREEN_HEIGHT;
			break;
		}
		return target;
	case TargetType::RETURN:
		target.first = ghost->getSpawnPos().first;
		target.second = ghost->getSpawnPos().second;
		return target;
	}
	return target;
}