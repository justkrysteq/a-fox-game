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
#define ANCHOR_TYPE_TOP_RIGHT 3

#define IMAGE_SOURCE_WIDTH 0
#define IMAGE_SOURCE_HEIGHT 0

#define KEY_QUIT SDLK_ESCAPE
#define KEY_MOVE_UP SDLK_w
#define KEY_MOVE_DOWN SDLK_s
#define KEY_MOVE_LEFT SDLK_a
#define KEY_MOVE_RIGHT SDLK_d
#define KEY_LIGHT_ATTACK SDLK_e
#define KEY_HEAVY_ATTACK SDLK_r
#define KEY_NEW_GAME SDLK_n

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
} Texture2D;

typedef struct {
	Texture2D player;
	Texture2D enemy;
	Texture2D background;
	Texture2D floor;
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
		case ANCHOR_TYPE_TOP_RIGHT:
			return x - w*scale;
	}
}

int get_y_offset(const int anchor_type, const int h, const int y, const float scale) {
	switch (anchor_type) {
		default:
		case ANCHOR_TYPE_TOP_RIGHT:
		case ANCHOR_TYPE_TOP_LEFT:
			return y;
		case ANCHOR_TYPE_CENTER:
			return y - h*scale / 2;
		case ANCHOR_TYPE_BOTTOM_LEFT:
			return y - h*scale;
	}
}

Texture2D create_text_texture(const char *text, TTF_Font *font, const SDL_Color color) {
	SDL_Surface *text_surface = TTF_RenderText_Solid(core()->font, text, color);
	check_sdl_failure(!text_surface, "Text could not be rendered");
	
	TTF_SizeText(font, text, &text_surface->w, &text_surface->h);

	SDL_Texture *text_texture = SDL_CreateTextureFromSurface(core()->renderer, text_surface);

	Texture2D text_texture2d = {
		.texture = text_texture,
		.w = text_surface->w,
		.h = text_surface->h
	};

	SDL_FreeSurface(text_surface);

	return text_texture2d;
}

void render_static_text(const Texture2D text, int x, int y, const int anchor_type) {
	x = get_x_offset(anchor_type, text.w, x, 1);
	y = get_y_offset(anchor_type, text.h, y, 1);

	SDL_Rect text_rect = {
		.x = x,
		.y = y,
		.w = text.w,
		.h = text.h
	};

	SDL_RenderCopy(core()->renderer, text.texture, NULL, &text_rect);
}

void render_dynamic_text(const char *text, const SDL_Color color, const int anchor_type, int x, int y) {
	SDL_Surface *text_surface = TTF_RenderText_Solid(core()->font, text, color);
	check_sdl_failure(!text_surface, "Text could not be rendered");

	TTF_SizeText(core()->font, text, &text_surface->w, &text_surface->h);

	SDL_Texture *text_texture = SDL_CreateTextureFromSurface(core()->renderer, text_surface);

	x = get_x_offset(anchor_type, text_surface->w, x, 1);
	y = get_y_offset(anchor_type, text_surface->h, y, 1);

	SDL_Rect text_rect = {
		.x = x,
		.y = y,
		.w = text_surface->w,
		.h = text_surface->h
	};

	SDL_RenderCopy(core()->renderer, text_texture, NULL, &text_rect);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
}

Texture2D create_sprite(const char *sprite_assets_path, int w, int h, const float scale) {
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

	Texture2D sprite = {
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

void render_sprite(Texture2D sprite, int x, int y, const float scale, const int anchor_type) {
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
	int fps = 0;
	int time_s = 0;

	int prev_frame_time = SDL_GetTicks64();
	int frame_time = 0;
	double delta_time = 0;

	float player_x = 100;

	char fps_text[16];
	char time_text[16];
	strcpy(fps_text, "FPS: ?");
	strcpy(time_text, "Time: 0");
	int fps_update_time = SDL_GetTicks64();

	Texture2D fps_text_texture = create_text_texture(fps_text, core()->font, colors()->white);
	Texture2D time_text_texture = create_text_texture(time_text, core()->font, colors()->white);

	while (state.running) {
		handle_sdl_events(&event, &state);

		frame_time = SDL_GetTicks64();
		delta_time = (double) (frame_time - prev_frame_time) / 1000;
		prev_frame_time = frame_time;

		SDL_RenderClear(core()->renderer);

		player_x += 50*delta_time;

		if (frame_time - fps_update_time >= 1000) {
			time_s++;

			sprintf(fps_text, "FPS: %d", fps);
			if (time_s < 60 && time_s >= 0) {
				sprintf(time_text, "Time: %d", time_s);
			} else if (time_s >= 60 && time_s < 3600) {
				if (time_s%60 < 10) {
					sprintf(time_text, "Time: %d:0%d", time_s/60, time_s%60);
				} else {
					sprintf(time_text, "Time: %d:%d", time_s/60, time_s%60);
				}
			}

			fps = 0;
			fps_update_time = frame_time;

			fps_text_texture = create_text_texture(fps_text, core()->font, colors()->white);
			time_text_texture = create_text_texture(time_text, core()->font, colors()->white);
		}

		render_static_text(fps_text_texture, 10, 10, ANCHOR_TYPE_TOP_LEFT);
		render_static_text(time_text_texture, WINDOW_WIDTH - 10, 10, ANCHOR_TYPE_TOP_RIGHT);

		render_sprite(sprites()->floor, 0, WINDOW_HEIGHT, 1, ANCHOR_TYPE_BOTTOM_LEFT);

		render_sprite(sprites()->player, (int) player_x, WINDOW_HEIGHT/2, 1, ANCHOR_TYPE_CENTER);

		SDL_RenderPresent(core()->renderer);

		fps++;
	}

	free((void *) colors());
	SDL_DestroyTexture(fps_text_texture.texture);
	SDL_DestroyTexture(time_text_texture.texture);

	free_core();
	free_sprites();

	SDL_Quit();

	return EXIT_SUCCESS;
}
