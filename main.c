#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void check_sdl_failure(const int condition, const char *message);

void init_sdl(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

		exit(EXIT_FAILURE);
	}

	check_sdl_failure(TTF_Init() != 0, "TTF could not initialize");
}

void check_sdl_failure(const int condition, const char *message) {
	if (condition) {
		SDL_Quit();
		printf("%s! SDL_Error: %s\n", message, SDL_GetError());

		exit(EXIT_FAILURE);
	}
}

int main(void) {
	init_sdl();

	SDL_Window *window = SDL_CreateWindow("Beat 'Em Up!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	check_sdl_failure(!window, "Window could not be created");

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	check_sdl_failure(!renderer, "Renderer could not be created");

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	TTF_Font* roboto = TTF_OpenFont("assets/Roboto-Regular.ttf", 32);
	check_sdl_failure(!roboto, "Font could not be opened");

	SDL_Color white = {255, 255, 255, 255};

	SDL_Surface *message_surface = TTF_RenderText_Solid(roboto, "Hello, world!", white);
	check_sdl_failure(!message_surface, "Text could not be rendered");

	TTF_SizeText(roboto, "Hello, world!", &message_surface->w, &message_surface->h);

	SDL_Texture *message_texture = SDL_CreateTextureFromSurface(renderer, message_surface);
	SDL_RenderCopy(renderer, message_texture, NULL, NULL);



	// SDL_Surface *surface = SDL_GetWindowSurface(window);
	// SDL_Surface *image = SDL_LoadBMP("assets/sprite.bmp");
	//
	// check_sdl_failure(!image, "Image could not be loaded");
	//
	// SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 40, 80, 255));
	//
	// SDL_Rect rect;
	// rect.w = 500;
	// rect.h = 50;
	// rect.x = (WINDOW_WIDTH - rect.w) / 2;
	// rect.y = (WINDOW_HEIGHT - rect.h) / 2;
	// SDL_BlitScaled(image, NULL, surface, &rect);

	SDL_RenderPresent(renderer);

	bool running = true;
	SDL_Event event;

	while (running) {

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_q) {
					running = false;
				}
			}
		}
	}

	// SDL_FreeSurface(image);
	// SDL_FreeSurface(surface);
	SDL_DestroyTexture(message_texture);
	SDL_FreeSurface(message_surface);
	TTF_CloseFont(roboto);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
