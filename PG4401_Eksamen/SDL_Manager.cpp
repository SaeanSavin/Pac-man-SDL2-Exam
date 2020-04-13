#include "SDL_Manager.h"

SDL_Manager::SDL_Manager() {}

void SDL_Manager::printError(std::ostream &os, const std::string &msg) {
	os << msg << " error " << SDL_GetError() << std::endl;
}

SDL_Window *SDL_Manager::createWindow(const char *c, const int w, const int h) {
	
	SDL_Window *window = SDL_CreateWindow(c,
										  SDL_WINDOWPOS_UNDEFINED,
										  SDL_WINDOWPOS_UNDEFINED,
										  w,
										  h,
										  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS
										  );
	if (window == nullptr) {
		printError(std::cout, "Cannot create window ");
		SDL_Quit();
		return nullptr;
	}

	return window;
}

SDL_Renderer *SDL_Manager::createRenderer(SDL_Window *w, int index) {
	SDL_Renderer *renderer = SDL_CreateRenderer(w, index, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr) {
		printError(std::cout, "Cannot create renederer ");
		SDL_DestroyWindow(w);
		SDL_Quit();
		return nullptr;
	}
	return renderer;
}

SDL_Surface *SDL_Manager::createSurface(const char *c, SDL_Window *window, SDL_Renderer *renderer) {
	SDL_Surface *surface = IMG_Load(c);

	if (surface == nullptr) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		return nullptr;
	}
	return surface;
}

void SDL_Manager::SetRenderColor(SDL_Renderer *renderer, int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}

void SDL_Manager::ClearRender(SDL_Renderer *renderer) {
	SDL_RenderClear(renderer);
}

void SDL_Manager::printFromTiles(std::string name, SDL_Renderer *r, SDL_Texture *t, SDL_Rect &dst, SDL_Rect &src) {

	for (size_t i = 0; i < name.size(); i++) {
		
		if (name[i] == ' ') {
			dst.x += 16;
			continue;
		}
		
		int x = name[i] - 65;
		int y = 0;

		//Goto next line in text.png
		if (x > 14) {
			x -= 15;
			y = 1;
		}

		src.x = x * 8;
		src.y = y * 8;
		dst.x += 16;

		SDL_RenderCopy(r, t, &src, &dst);
	}
}

void SDL_Manager::printPlayerScore(int score, SDL_Renderer *r, SDL_Texture *t, SDL_Rect &dst, SDL_Rect &src) {


	int x = score;
	int length = 1;
	src.y = 2 * 8;

	while (x / 10 != 0 && x % 10 != 0) {

		int rest = x / 10;
		src.x = rest * 8;
		SDL_RenderCopy(r, t, &src, &dst);
		x -= 10;
		dst.x += 16;
	}

	src.x = x * 8;
	SDL_RenderCopy(r, t, &src, &dst);
	dst.x += 16;
}