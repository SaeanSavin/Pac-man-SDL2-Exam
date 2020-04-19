#include "GameManager.h"

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

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
	
	auto sdlManager = std::make_shared<SDL_Manager>();
	auto textureManager = std::make_shared<Texture_Manager>();
	
	SDL_Window* window = sdlManager->createWindow("Pac-man", SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_Renderer* renderer = sdlManager->createRenderer(window, -1);

	//draws background
	sdlManager->SetRenderColor(renderer, 0, 0, 0, 255);
	sdlManager->clearRenderer(renderer);

	//Sets focus on window
	SDL_RaiseWindow(window);

	/*   HUD SETUP   */

	std::cout << "building HUD..." << std::endl;

	//Text 
	SDL_Surface *textSurface = sdlManager->createSurface("../images/TextTiles/text.png", window, renderer);
	SDL_Texture *text = textureManager->draw(renderer, textSurface);

	//HP 
	SDL_Surface *hpSurface = sdlManager->createSurface("../images/Pacman/move/2.png", window, renderer);
	SDL_Texture *hpTexture = textureManager->draw(renderer, hpSurface);

	/*    VECTORS   */

	//Wall vector
	std::vector<SDL_Rect> walls{};

	//pellet vector
	std::vector<SDL_Rect> edible{};
	
	//walkable vector
	std::vector<SDL_Rect> walkable{};

	SDL_Rect mapRect = sdlManager->createRect(16, 16, 0, 50);

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
			case 'A' :
				edible.emplace_back(mapRect);
				walkable.emplace_back(mapRect);
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

	/*   AUDIO SETUP   */

	//channel 1 is reserved for music
	//channel 2 is reserved for sound effects

	std::cout << "preparing audio..." << std::endl;

	Mix_Chunk* introSound = NULL;
	Mix_Chunk* bgMusic = NULL;
	Mix_Chunk* powMusic = NULL;
	Mix_Chunk* eatSound = NULL;
	Mix_Chunk* deathSound = NULL;
	Mix_Chunk* ghostEatSound = NULL;

	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

	introSound = Mix_LoadWAV("../audio/intro_2.wav");
	eatSound = Mix_LoadWAV("../audio/eat.wav");
	deathSound = Mix_LoadWAV("../audio/death.wav");
	ghostEatSound = Mix_LoadWAV("../audio/eatghost.wav");
	bgMusic = Mix_LoadWAV("../audio/background.wav");
	powMusic = Mix_LoadWAV("../audio/powered.wav");

	/* CHARACTER SETUP  */

	std::cout << "building characters..." << std::endl;


	//build players, ghosts and set spawn positions
	std::vector<std::shared_ptr<Player>> players;
	int pIndex = 0;
	std::vector<std::shared_ptr<Ghost>> ghosts;
	int ghostNr = 1;
	int gIndex = 0;
	SDL_Rect mPosition = sdlManager->createRect(16, 16, 0, 50);

	for (auto& row : map) {
		for (auto& c : row) {
			switch (c) {
				case 'G':
					switch (ghostNr) {
						case 1:
							ghosts.emplace_back(makeGhost(textureManager, renderer, walkable, GhostType::SHADOW));
							ghosts[gIndex]->setSpawnPos(mPosition.x, mPosition.y);
							ghostNr++;
							gIndex++;
							break;
						case 2:
							ghosts.emplace_back(makeGhost(textureManager, renderer, walkable, GhostType::SPEEDY));
							ghosts[gIndex]->setSpawnPos(mPosition.x, mPosition.y);
							ghostNr++;
							gIndex++;
							break;
						case 3:
							ghosts.emplace_back(makeGhost(textureManager, renderer, walkable, GhostType::BASHFUL));
							ghosts[gIndex]->setSpawnPos(mPosition.x, mPosition.y);
							ghostNr++;
							gIndex++;
							break;
						case 4:
							ghosts.emplace_back(makeGhost(textureManager, renderer, walkable, GhostType::POKEY));
							ghosts[gIndex]->setSpawnPos(mPosition.x, mPosition.y);
							ghostNr = 1;
							gIndex++;
							break;
					}
					c = '~';
					break;
				case 'S':
					players.emplace_back(makePlayer(textureManager, sdlManager, renderer, keys, edible, walkable, eatSound, pIndex));
					players[pIndex]->setSpawnPos(mPosition.x, mPosition.y);
					pIndex++;
					break;
			}
			mPosition.x += 16;
		}
		mPosition.x = 0;
		mPosition.y += 16;
	}

	//Freeing the RGB surface
	SDL_FreeSurface(textSurface);


	/*   TEXTURE SETUP   */
	
	std::cout << "loading assets..." << std::endl;

	//Create Textures
	SDL_Texture *pellet = textureManager->loadTexture("../images/Edible/pellet.png", renderer);
	SDL_Texture *cherry = textureManager->loadTexture("../images/Edible/cherry.png", renderer);
	SDL_Texture *apple = textureManager->loadTexture("../images/Edible/apple.png", renderer);
	SDL_Texture *poweups = textureManager->loadTexture("../images/Edible/powerup.png", renderer);
	SDL_Texture *wallBottom = textureManager->loadTexture("../images/mapTiles/wall_bottom_single.png", renderer);
	SDL_Texture *wallTop = textureManager->loadTexture("../images/mapTiles/wall_top_single.png", renderer);
	SDL_Texture *wallLeft = textureManager->loadTexture("../images/mapTiles/wall_left_single.png", renderer);
	SDL_Texture *wallRight = textureManager->loadTexture("../images/mapTiles/wall_right_single.png", renderer);
	SDL_Texture *cornerTopRight = textureManager->loadTexture("../images/mapTiles/wall_corner_tr_single.png", renderer);
	SDL_Texture *cornerTopLeft = textureManager->loadTexture("../images/mapTiles/wall_corner_tl_single.png", renderer);
	SDL_Texture *cornerBottomRight = textureManager->loadTexture("../images/mapTiles/wall_corner_br_single.png", renderer);
	SDL_Texture *cornerBottomLeft = textureManager->loadTexture("../images/mapTiles/wall_corner_bl_single.png", renderer);

	SDL_Rect textSrc = sdlManager->createRect(8, 8, 0, 0);

	SDL_Rect readyDst = sdlManager->createRect(16, 16, SCREEN_WIDTH / 2 - (startText.length() * 8), SCREEN_HEIGHT / 2);
	textureManager->printFromTiles(startText, renderer, text, readyDst, textSrc);
	sdlManager->clearAndUpdateRenderer(renderer);

	/*    VARIABLES   */

	int score = 0;
	int health = 3;

	//manage pellets
	int currentPellets = 0;

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


	//play intro
	Mix_PlayChannel(1, introSound, 0);

	while (Mix_Playing(1));

	/*   GAME LOOP START  */

	while (isRunning) {

		if (!Mix_Playing(1)) {
			Mix_PlayChannel(1, bgMusic, -1);
		}

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

		SDL_Rect textDst = sdlManager->createRect(16,16, 0, 25);

		textureManager->printFromTiles("SCORE ", renderer, text, textDst, textSrc);

		score = 0;
		for (auto& p : players) {
			score += p->getScore();
		}

		textureManager->printPlayerScore(score, renderer, text, textDst, textSrc);

		SDL_Rect hpDst = sdlManager->createRect(16, 16, 0, SCREEN_HEIGHT - 25);
		textureManager->printFromTiles("LIVES ", renderer, text, hpDst, textSrc);
		
		// collision check for each player with each ghost
		for (auto &p : players) {
			//first get pacman coords once in this thread
			int pCoordsLeft = p->getCoords()->x;
			int pCoordsRight = p->getCoords()->x + p->getCoords()->w;
			int pCoordsUp = p->getCoords()->y;
			int pCoordsDown = p->getCoords()->y + p->getCoords()->h;

			bool hit = false;

			//run collision check with each ghost parallel
			std::for_each(
				std::execution::par_unseq,
				ghosts.begin(),
				ghosts.end(),
				[&pCoordsLeft, &pCoordsRight, &pCoordsUp, &pCoordsDown, &p, &isPowered, &poweredStart, ghosts, &isRunning, deathSound, ghostEatSound, &hit]
			(auto& g) {
				int gCoordsLeft = g->getCoords()->x;
				int gCoordsRight = g->getCoords()->x + g->getCoords()->w;
				int gCoordsUp = g->getCoords()->y;
				int gCoordsDown = g->getCoords()->y + g->getCoords()->h;

				if (pCoordsLeft < gCoordsRight && pCoordsRight > gCoordsLeft) {
					if (pCoordsUp < gCoordsDown && pCoordsDown > gCoordsUp) {
						//if pacman hit frightened ghosts
						if (g->isFrightened()) {
							g->hitByPacman();
							Mix_PlayChannel(3, ghostEatSound, 0);
							while (Mix_Playing(3));
							p->addScore(1000);
						}
						//if pacman hit ghosts normally
						else if (!g->isEaten()) {
							hit = true;
						}
					}
				}
			});

			if (hit) {
				Mix_PlayChannel(2, deathSound, 0);
				p->hitByGhost();
				isPowered = false;
				poweredStart = 0;
				if (Mix_Playing(1)) {
					Mix_HaltChannel(1);
				}
				for (auto& g : ghosts) {
					g->respawn();
				}
			}
		}

		if (players.size() > 1) {
			auto end = std::remove_if(players.begin(),
				players.end(),
				[](auto& p) {
				return p->getHP() <= 0;
			});

			players.erase(end, players.end());
		}

		health = 0;
		for (auto& p : players) {
			if (p->getHP() <= 0) {
				continue;
			}
			for (int i = 0; i < p->getHP(); i++) {
				SDL_RenderCopy(renderer, hpTexture, nullptr, &hpDst);
				hpDst.x += 20;
				health++;
			}
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

		for (auto& p : players) {
			//Move characters and check if pacman is powered
			p->move(SCREEN_WIDTH, SCREEN_HEIGHT, map, walls);

			//runs on the frame that pacman gets power pellet
			if (p->isPowered()) {
				Mix_PlayChannel(1, powMusic, -1);
				p->stopPowered();
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
					Mix_HaltChannel(1);
					Mix_PlayChannel(1, bgMusic, -1);
					isPowered = false;
					for (auto& g : ghosts) {
						if (g->isFrightened()) {
							g->stopFrightened();
						}
					}
					poweredStart = 0;
				}
			}
		}

		//decide target type for ghosts and move ghosts accordingly
		size_t gIndex = 0;
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
					g->setTarget(getTarget(g->getTargetMode(), targetClosestPlayer(g, players), g));
				}
				else if (g->getTargetMode() == TargetType::SUPPORTIVE) {
					g->setTarget(getTarget(g->getTargetMode(), targetClosestPlayer(g, players), ghosts[gIndex - 2]));
				}
				else {
					g->setTarget(getTarget(g->getTargetMode(), targetClosestPlayer(g, players)));
				}
			}
			g->move(SCREEN_WIDTH, SCREEN_HEIGHT, map, walls);
			gIndex++;
		}

		//render map
		SDL_Rect mapRect = sdlManager->createRect(16, 16, 0, 50);

		currentPellets = 0;

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
				case 'A':
					SDL_RenderCopy(renderer, apple, nullptr, &mapRect);
					break;
				case 'P':
					SDL_RenderCopy(renderer, poweups, nullptr, &mapRect);
					break;
				case '1':
					SDL_RenderCopy(renderer, cornerBottomLeft, nullptr, &mapRect);
					break;
				case '3':
					SDL_RenderCopy(renderer, cornerBottomRight, nullptr, &mapRect);
					break;
				case '7':
					SDL_RenderCopy(renderer, cornerTopLeft, nullptr, &mapRect);
					break;
				case '9':
					SDL_RenderCopy(renderer, cornerTopRight, nullptr, &mapRect);
					break;
				case '2':
					SDL_RenderCopy(renderer, wallBottom, nullptr, &mapRect);
					break;
				case '4':
					SDL_RenderCopy(renderer, wallLeft, nullptr, &mapRect);
					break;
				case '6':
					SDL_RenderCopy(renderer, wallRight, nullptr, &mapRect);
					break;
				case '8':
					SDL_RenderCopy(renderer, wallTop, nullptr, &mapRect);
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

		sdlManager->clearAndUpdateRenderer(renderer);

		//game over
		if (health <= 0) {
			SDL_Rect gameoverDst = sdlManager->createRect(16, 16, SCREEN_WIDTH / 2 - (gameoverText.length() * 8), SCREEN_HEIGHT / 2);
			textureManager->printFromTiles(gameoverText, renderer, text, gameoverDst, textSrc);
			sdlManager->clearAndUpdateRenderer(renderer);
			isRunning = false;
			SDL_Delay(2000);
		}

		//level complete
		if (currentPellets <= 0 && health > 0) {
			Mix_HaltChannel(-1);
			SDL_Rect levelcompletedDst = sdlManager->createRect(16, 16, SCREEN_WIDTH / 2 - (levelcompletedText.length() * 8), SCREEN_HEIGHT / 2);
			textureManager->printFromTiles(levelcompletedText, renderer, text, levelcompletedDst, textSrc);
			sdlManager->clearAndUpdateRenderer(renderer);
			for (auto& p : players) {
				p->respawn();
			}
			for (auto &g : ghosts) {
				g->respawn();
			}
			map = defaultMap;
			SDL_Delay(2000);
		}
	}

	/*   GAME LOOP END   */

	//Program exit 
	Mix_FreeChunk(bgMusic);
	Mix_CloseAudio();
	SDL_GameControllerClose(gameController);
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

//returns a Player type pointer
std::shared_ptr<Player> GameManager::makePlayer(std::shared_ptr<Texture_Manager> textureManager, std::shared_ptr<SDL_Manager> sdlManager, SDL_Renderer* renderer, const Uint8 *keys, std::vector<SDL_Rect>& edible, std::vector<SDL_Rect>& walkable, Mix_Chunk *eatSound, int index) {
	SDL_Texture* pacTexture = textureManager->loadTexture("../images/pacman/move/1.png", renderer);
	SDL_GameController* gameController = sdlManager->getGameController(index);

	std::shared_ptr<Player> p = std::make_shared<Player>(pacTexture, renderer, keys, edible, gameController, walkable, index);
	p->setPos(0, 0);
	p->setSize(16, 16);

	auto pacmanMove = std::make_shared<Animation>(renderer, "../images/Pacman/move", 12);
	p->setAnimation("move", pacmanMove);

	auto pacmanDead = std::make_shared<Animation>(renderer, "../images/Pacman/dead", 120);
	p->setAnimation("dead", pacmanDead);

	p->setSound(eatSound);

	return p;
}

//returns a Ghost class pointer
std::shared_ptr<Ghost> GameManager::makeGhost(std::shared_ptr<Texture_Manager> textureManager, SDL_Renderer* renderer, std::vector<SDL_Rect>& walkable, GhostType type) {
	std::string ghostPath = "../images/Ghosts/";
	std::string ghostAnimPath = "../images/Ghosts/";

	TargetType mode;

	switch (type) {
		case GhostType::SHADOW:
			ghostPath += "Shadow/shadow.png";
			ghostAnimPath += "Shadow/";
			mode = TargetType::AGRESSIVE;
			break;
		case GhostType::SPEEDY:
			ghostPath += "Speedy/speedy.png";
			ghostAnimPath += "Speedy/";
			mode = TargetType::AMBUSH;
			break;
		case GhostType::BASHFUL:
			ghostPath += "Bashful/bashful.png";
			ghostAnimPath += "Bashful/";
			mode = TargetType::SUPPORTIVE;
			break;
		case GhostType::POKEY:
			ghostPath += "Pokey/pokey.png";
			ghostAnimPath += "Pokey/";
			mode = TargetType::EVASIVE;
			break;
	}

	SDL_Texture* ghostTexture = textureManager->loadTexture(&ghostPath[0], renderer);

	auto ghost = std::make_shared<Ghost>(ghostTexture, renderer, walkable, mode, type);
	ghost->setPos(32, 0);
	ghost->setSize(16, 16);

	auto ghostUp = std::make_shared<Animation>(renderer, (ghostAnimPath + "move/up"), 12);
	auto ghostDown = std::make_shared<Animation>(renderer, (ghostAnimPath + "move/down"), 12);
	auto ghostLeft = std::make_shared<Animation>(renderer, (ghostAnimPath + "move/left"), 12);
	auto ghostRight = std::make_shared<Animation>(renderer, (ghostAnimPath + "move/right"), 12);

	auto frightened = std::make_shared<Animation>(renderer, "../images/Ghosts/Frightened/move", 12);
	auto frightenedEnding = std::make_shared<Animation>(renderer, "../images/Ghosts/Frightened/move_last", 12);

	auto eyesUp = std::make_shared<Animation>(renderer, "../images/Ghosts/Eyes/move/up", 12);
	auto eyesDown = std::make_shared<Animation>(renderer, "../images/Ghosts/Eyes/move/down", 12);
	auto eyesLeft = std::make_shared<Animation>(renderer, "../images/Ghosts/Eyes/move/left", 12);
	auto eyesRight = std::make_shared<Animation>(renderer, "../images/Ghosts/Eyes/move/right", 12);

	ghost->setAnimation("up", ghostUp);
	ghost->setAnimation("down", ghostDown);
	ghost->setAnimation("left", ghostLeft);
	ghost->setAnimation("right", ghostRight);
	
	ghost->setAnimation("frightened", frightened);
	ghost->setAnimation("frightened_ending", frightenedEnding);

	ghost->setAnimation("eaten_up", eyesUp);
	ghost->setAnimation("eaten_down", eyesDown);
	ghost->setAnimation("eaten_left", eyesLeft);
	ghost->setAnimation("eaten_right", eyesRight);

	return ghost;
}

//selects which player the ghosts should target
std::shared_ptr<Player> GameManager::targetClosestPlayer(std::shared_ptr<Ghost> ghost, std::vector<std::shared_ptr<Player>> &players) {

	if (players.size() == 1) {
		return players[0];
	}

	int gx = ghost->getCoords()->x;
	int gy = ghost->getCoords()->y;
	double closestPlayer = closestPlayer = std::sqrt(std::pow(abs(players[0]->getCoords()->x - gx), 2) + std::pow(abs(players[0]->getCoords()->y - gy), 2));;
	double nextPlayer = 0;

	std::shared_ptr<Player> target = players[0];

	for (auto& p : players) {
		nextPlayer = std::sqrt(std::pow(abs(p->getCoords()->x - gx), 2) + std::pow(abs(p->getCoords()->y - gy), 2));
		if (nextPlayer <= closestPlayer) {
			closestPlayer = nextPlayer;
			target = p;
		}
	}

	return target;

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
	int vX = 0;
	int vY = 0;
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
			vX = abs(ghost->getCoords()->x - target.first);
			vY = abs(ghost->getCoords()->y - target.second);

			//finally, rotate vector 180 degrees and set target to this position
			target.first += vX * -1;
			target.second += vY * -1;
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
	return target;
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