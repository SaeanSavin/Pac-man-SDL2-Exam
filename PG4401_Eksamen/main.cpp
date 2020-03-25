#include <iostream>
#include <SDL.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

void printError(std::ostream &os, const std::string &msg) {
	os << msg << " error " << SDL_GetError() << std::endl;
}

int main(int argc, char *argv[]) {
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = nullptr;

	window = SDL_CreateWindow(

		"Pac-man",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_FOCUS
	);

	if (window == nullptr) {
		printError(std::cout, "Failed to create window: ");
		SDL_Quit();
		return EXIT_FAILURE;
	}
	
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr) {
		printError(std::cout, "Failed to create renderer: ");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_Surface *surface = SDL_LoadBMP("../images/my_picture.bmp");

	if (surface == nullptr) {
		printError(std::cout, "Failed to load image: ");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	SDL_Texture *drawable = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect coords;
	coords.h = surface->h;
	coords.w = surface->w;
	coords.x = 0;
	coords.y = 100;

	SDL_FreeSurface(surface);

	bool isRunning = true;
	const Uint8 *keys = nullptr;
	int numKeys;
	keys = SDL_GetKeyboardState(&numKeys);
	SDL_Event evt;

	while (isRunning) {

		if (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_KEYDOWN) {
				if (evt.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false;
				}
			} else if (evt.type == SDL_QUIT) {
				isRunning = false;
			}
		}

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


		SDL_RenderCopy(renderer, drawable, nullptr, &coords);

		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}


	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}