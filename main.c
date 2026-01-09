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

#define KEY_QUIT SDLK_q
#define KEY_MOVE_UP SDLK_w
#define KEY_MOVE_DOWN SDLK_s
#define KEY_MOVE_LEFT SDLK_a
#define KEY_MOVE_RIGHT SDLK_d
#define KEY_LIGHT_ATTACK SDLK_e
#define KEY_HEAVY_ATTACK SDLK_r

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

typedef struct {
	SDL_Texture *texture;
	int w, h;
} Sprite;

typedef struct {
	Sprite player;
	Sprite enemy;
	Sprite background;
	Sprite floor;
} Sprites;

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
			switch (event->key.keysym.sym) {
				case KEY_QUIT:
					state->running = false;
					break;
				case KEY_MOVE_UP:
					break;
				case KEY_MOVE_DOWN:
					break;
				case KEY_MOVE_LEFT:
					break;
				case KEY_MOVE_RIGHT:
					break;
				case KEY_LIGHT_ATTACK:
					break;
				case KEY_HEAVY_ATTACK:
					break;
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
		colors = (Colors *) malloc(sizeof(Colors));
		check_malloc_failure(colors);

		colors->white = (SDL_Color) {255, 255, 255, 255};
	}

	return colors;
}

const Core *core(void) {
	static Core *core = NULL;

	if (!core) {
		core = (Core *) malloc(sizeof(Core));
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

// void render_static_text(const char *text, const SDL_Color color, const int anchor_type, int x, int y) {
//
// }

void render_changable_text(const char *text, const SDL_Color color, const int anchor_type, int x, int y) {
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

Sprite create_sprite(const char *sprite_assets_path, int w, int h, const float scale) {
	char *sprite_path = (char *) malloc(sizeof(char) * (strlen(sprite_assets_path) + 1 + strlen("assets/")));
	check_malloc_failure(sprite_path);

	strcpy(sprite_path, "assets/");
	strcat(sprite_path, sprite_assets_path);

	SDL_Surface *sprite_surface = SDL_LoadBMP(sprite_path);
	check_sdl_failure(!sprite_surface, "Sprite could not be loaded");

	if (w == IMAGE_SOURCE_WIDTH) {
		w = sprite_surface->w;
	}

	if (h == IMAGE_SOURCE_HEIGHT) {
		h = sprite_surface->h;
	}

	SDL_Texture *sprite_texture = SDL_CreateTextureFromSurface(core()->renderer, sprite_surface);

	SDL_FreeSurface(sprite_surface);

	free(sprite_path);

	Sprite sprite = {
		.texture = sprite_texture,
		.w = (int) (w * scale),
		.h = (int) (h * scale)
	};

	return sprite;
}

const Sprites *sprites(void) {
	static Sprites *sprites = NULL;

	if (!sprites) {
		sprites = (Sprites *) malloc(sizeof(Sprites));
		check_malloc_failure(sprites);

		sprites->player = create_sprite("sprite.bmp", IMAGE_SOURCE_WIDTH, IMAGE_SOURCE_WIDTH, 0.25);
		sprites->floor = create_sprite("floor.bmp", WINDOW_WIDTH, (int) (0.6*WINDOW_HEIGHT), 1);
	}

	return sprites;
}

void free_sprites(void) {
	if (sprites()) {
		SDL_DestroyTexture(sprites()->player.texture);
		// SDL_DestroyTexture(sprites()->enemy.texture);
		// SDL_DestroyTexture(sprites()->background.texture);
		SDL_DestroyTexture(sprites()->floor.texture);

		free((void *) sprites());
	}
}

void render_sprite(Sprite sprite, int x, int y, const float scale, const int anchor_type) {
	x = get_x_offset(anchor_type, sprite.w, x, scale);
	y = get_y_offset(anchor_type, sprite.h, y, scale);

	SDL_Rect sprite_rect = {
		.x = x,
		.y = y,
		.w = (int) (sprite.w * scale),
		.h = (int) (sprite.h * scale)
	};

	SDL_RenderCopy(core()->renderer, sprite.texture, NULL, &sprite_rect);
}



int main(void) {
	init_sdl();

	State state = init_state();



	SDL_Event event;
	int i = 0;

	while (state.running) { // TODO: Implement delta time
		handle_sdl_events(&event, &state);

		SDL_RenderClear(core()->renderer);

		SDL_SetRenderDrawColor(core()->renderer, 0, 0, 0, 255);
		render_changable_text("Hello, world!", colors()->white, ANCHOR_TYPE_CENTER, WINDOW_WIDTH/2, 100);

		render_sprite(sprites()->floor, 0, WINDOW_HEIGHT, 1, ANCHOR_TYPE_BOTTOM_LEFT);

		render_sprite(sprites()->player, 100+i, WINDOW_HEIGHT/2, 1, ANCHOR_TYPE_CENTER);

		SDL_RenderPresent(core()->renderer);

		i++;
		SDL_Delay(10);
	}

	free((void *) colors());

	free_core();
	free_sprites();

	SDL_Quit();

	return EXIT_SUCCESS;
}
