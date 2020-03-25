#include <iostream>
#include <SDL.h>

//Window size
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

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



int main(int argc, char *argv[]) {
	
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	//Initizing SDL Video subsystem
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = nullptr;

	//Creates Window
	window = SDL_CreateWindow(

		"Pac-man",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_BORDERLESS
	);

	//Checks if window exist
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
	SDL_Surface *surface = SDL_LoadBMP("../images/my_picture.bmp");

	//Checks if a surface exist
	if (surface == nullptr) {
		printError(std::cout, "Failed to load image: ");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
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

	bool isRunning = true;
	const Uint8 *keys = nullptr;
	int numKeys;
	keys = SDL_GetKeyboardState(&numKeys);
	SDL_Event evt;

	//movement variables
	char direction = 'i';

	//Game Loop
	while (isRunning) {


		frameStart = SDL_GetTicks();

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

		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	//Program exit 
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}