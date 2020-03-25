#include <iostream>
#include <SDL.h>

//Window size
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//Error function
void printError(std::ostream &os, const std::string &msg) {
	os << msg << " error " << SDL_GetError() << std::endl;
}

int main(int argc, char *argv[]) {
	
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
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_FOCUS
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

	//Game Loop
	while (isRunning) {


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
		SDL_PumpEvents();

		if (keys[SDL_SCANCODE_W]) {
			coords.y -= 1;
		}

		if (keys[SDL_SCANCODE_S]) {
			coords.y += 1;
		}

		if (keys[SDL_SCANCODE_A]) {
			coords.x -= 1;
		}

		if (keys[SDL_SCANCODE_D]) {
			coords.x += 1;
		}

		//Make sure image dont move out of bounds

		if (coords.x > SCREEN_WIDTH - surface->w) {
			coords.x = SCREEN_WIDTH - surface->w;
		}

		if (coords.y > SCREEN_HEIGHT - surface->h) {
			coords.y = SCREEN_HEIGHT - surface->h;
		}

		if (coords.x < 0) {
			coords.x = 0;
		}

		if (coords.y < 0) {
			coords.y = 0;
		}


		//Prepare Renderer for a new frame
		SDL_RenderCopy(renderer, drawable, nullptr, &coords);

		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}

	//Program exit 
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}