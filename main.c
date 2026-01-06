#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define GAME_TITLE "Beat 'Em Up!"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define FONT_SIZE 32

#define ANCHOR_TYPE_CENTER 0
#define ANCHOR_TYPE_TOP_LEFT 1
#define ANCHOR_TYPE_BOTTOM_LEFT 2

#define IMAGE_SOURCE_WIDTH 0
#define IMAGE_SOURCE_HEIGHT 0

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_Font *font;
} Core;

typedef struct {
	bool running;
} State;

typedef struct {
	SDL_Color white;
} Colors;

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

void handle_sdl_events(SDL_Event *event, State *state) {
	while (SDL_PollEvent(event)) {
		if (event->type == SDL_QUIT) {
			state->running = false;
		} else if (event->type == SDL_KEYDOWN) {
			if (event->key.keysym.sym == SDLK_q) {
				state->running = false;
			}
		}
	}
}

void check_malloc_failure(const void *pointer) {
	if (!pointer) {
		printf("malloc failed!\n");

		exit(EXIT_FAILURE);
	}
}

const Colors *colors(void) { // NOTE: Might be better to use global consts
	static Colors *colors = NULL;

	if (!colors) {
		colors = malloc(sizeof(Colors));
		check_malloc_failure(colors);

		colors->white = (SDL_Color) {255, 255, 255, 255};
	}

	return colors;
}

const Core *core(void) {
	static Core *core = NULL;

	if (!core) {
		core = malloc(sizeof(Core));
		check_malloc_failure(core);

		core->window = NULL;
		core->renderer = NULL;
		core->font = NULL;

		core->window = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		check_sdl_failure(!core->window, "Window could not be created");

		core->renderer = SDL_CreateRenderer(core->window, -1, SDL_RENDERER_ACCELERATED);
		check_sdl_failure(!core->renderer, "Renderer could not be created");

		core->font = TTF_OpenFont("assets/KiwiSoda.ttf", FONT_SIZE);
		check_sdl_failure(!core->font, "Font could not be opened");
	}

	return core;
}

void free_core(void) {
	if (core()) {
		if (core()->font) {
			TTF_CloseFont(core()->font);
		}

		if (core()->renderer) {
			SDL_DestroyRenderer(core()->renderer);
		}

		if (core()->window) {
			SDL_DestroyWindow(core()->window);
		}

		free((void *) core());
		TTF_Quit();
	}
}

State init_state(void) {
	State state = {
		.running = true
	};

	return state;
}

int get_x_offset(const int anchor_type, const int w, const int x, const float scale) {
	switch (anchor_type) {
		default:
		case ANCHOR_TYPE_TOP_LEFT:
		case ANCHOR_TYPE_BOTTOM_LEFT:
			return x;
		case ANCHOR_TYPE_CENTER:
			return x - w*scale / 2;
	}
}

int get_y_offset(const int anchor_type, const int h, const int y, const float scale) {
	switch (anchor_type) {
		default:
		case ANCHOR_TYPE_TOP_LEFT:
			return y;
		case ANCHOR_TYPE_CENTER:
			return y - h*scale / 2;
		case ANCHOR_TYPE_BOTTOM_LEFT:
			return y - h*scale;
	}
}

void render_text(const char *text, const SDL_Color color, const int anchor_type, int x, int y) {
	SDL_Surface *message_surface = TTF_RenderText_Solid(core()->font, "Hello, world!", color);
	check_sdl_failure(!message_surface, "Text could not be rendered");

	TTF_SizeText(core()->font, text, &message_surface->w, &message_surface->h);

	SDL_Texture *message_texture = SDL_CreateTextureFromSurface(core()->renderer, message_surface);

	x = get_x_offset(anchor_type, message_surface->w, x, 1);
	y = get_y_offset(anchor_type, message_surface->h, y, 1);

	SDL_Rect message_rect = {
		.x = x,
		.y = y,
		.w = message_surface->w,
		.h = message_surface->h
	};

	SDL_RenderCopy(core()->renderer, message_texture, NULL, &message_rect);

	SDL_FreeSurface(message_surface);
	SDL_DestroyTexture(message_texture);
}

void render_image(const char *image_assets_path, const int anchor_type, int x, int y, int w, int h, const float scale) {
	char *image_path = malloc(sizeof(char) * (strlen(image_assets_path) + 1 + strlen("assets/")));
	check_malloc_failure(image_path);

	strcpy(image_path, "assets/");
	strcat(image_path, image_assets_path);

	SDL_Surface *image_surface = SDL_LoadBMP(image_path);
	check_sdl_failure(!image_surface, "Image could not be loaded");

	if (w == IMAGE_SOURCE_WIDTH) {
		w = image_surface->w;
	}

	if (h == IMAGE_SOURCE_HEIGHT) {
		h = image_surface->h;
	}

	x = get_x_offset(anchor_type, w, x, scale);
	y = get_y_offset(anchor_type, h, y, scale);

	SDL_Rect image_rect = {
		.x = x,
		.y = y,
		.w = w*scale,
		.h = h*scale
	};

	SDL_Texture *image_texture = SDL_CreateTextureFromSurface(core()->renderer, image_surface);
	SDL_RenderCopy(core()->renderer, image_texture, NULL, &image_rect);

	SDL_FreeSurface(image_surface);
	SDL_DestroyTexture(image_texture);

	free(image_path);
}

int main(void) {
	init_sdl();

	State state = init_state();



	SDL_Event event;
	int i = 0;

	while (state.running) {
		handle_sdl_events(&event, &state);
		SDL_RenderClear(core()->renderer);

		SDL_SetRenderDrawColor(core()->renderer, 0, 0, 0, 255);
		render_text("Hello, world!", colors()->white, ANCHOR_TYPE_CENTER, WINDOW_WIDTH/2, 100);

		render_image("floor.bmp", ANCHOR_TYPE_BOTTOM_LEFT, 0, WINDOW_HEIGHT, WINDOW_WIDTH, (int) (0.6*WINDOW_HEIGHT), 1);

		render_image("sprite.bmp", ANCHOR_TYPE_CENTER, 100+i, WINDOW_HEIGHT/2, IMAGE_SOURCE_WIDTH, IMAGE_SOURCE_WIDTH, 0.25);

		SDL_RenderPresent(core()->renderer);

		i++;
		SDL_Delay(10);
	}

	free((void *) colors());

	free_core();

	SDL_Quit();

	return EXIT_SUCCESS;
}
