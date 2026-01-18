#include "core.h"

const Core *core(void) {
	static Core *core = NULL;

	if (!core) {
		core = (Core *) malloc(sizeof(Core));

		core->window = NULL;
		core->renderer = NULL;
		core->font = NULL;

		core->window = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		check_sdl_failure(!core->window, "Window could not be created");

		core->renderer = SDL_CreateRenderer(core->window, -1, SDL_RENDERER_ACCELERATED);
		check_sdl_failure(!core->renderer, "Renderer could not be created");

		core->font = TTF_OpenFont("KiwiSoda.ttf", FONT_SIZE);
		check_sdl_failure(!core->font, "Font could not be opened");
	}

	return core;
}

void free_core(void) {
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

void init_sdl(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

		exit(EXIT_FAILURE);
	}

	check_sdl_failure(TTF_Init() != 0, "TTF could not initialize");
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

Texture2D create_sprite(const char *sprite_path, int w, int h, const float scale) {
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

		sprites->floor = create_sprite("floor.bmp", WINDOW_WIDTH, (int) FLOOR_HEIGHT, 1);
		sprites->background = create_sprite("background.bmp", IMAGE_SOURCE_WIDTH, IMAGE_SOURCE_HEIGHT, 1);

		sprites->fox_u = create_sprite("fox_u.bmp", IMAGE_SOURCE_WIDTH, IMAGE_SOURCE_WIDTH, 3);
		sprites->fox_d = create_sprite("fox_d.bmp", IMAGE_SOURCE_WIDTH, IMAGE_SOURCE_WIDTH, 3);
		sprites->fox_l = create_sprite("fox_l.bmp", IMAGE_SOURCE_WIDTH, IMAGE_SOURCE_WIDTH, 3);
		sprites->fox_r = create_sprite("fox_r.bmp", IMAGE_SOURCE_WIDTH, IMAGE_SOURCE_WIDTH, 3);
		sprites->fox_idle = create_sprite("fox_idle.bmp", IMAGE_SOURCE_WIDTH, IMAGE_SOURCE_WIDTH, 3);
		sprites->fox_a1 = create_sprite("fox_a1.bmp", IMAGE_SOURCE_WIDTH, IMAGE_SOURCE_WIDTH, 3);
		sprites->fox_a2 = create_sprite("fox_a2.bmp", IMAGE_SOURCE_WIDTH, IMAGE_SOURCE_WIDTH, 3);
	}

	return sprites;
}

void free_sprites(void) {
	if (sprites()) {
		SDL_DestroyTexture(sprites()->background.texture);
		SDL_DestroyTexture(sprites()->floor.texture);

		SDL_DestroyTexture(sprites()->fox_u.texture);
		SDL_DestroyTexture(sprites()->fox_d.texture);
		SDL_DestroyTexture(sprites()->fox_l.texture);
		SDL_DestroyTexture(sprites()->fox_r.texture);
		SDL_DestroyTexture(sprites()->fox_idle.texture);
		SDL_DestroyTexture(sprites()->fox_a1.texture);
		SDL_DestroyTexture(sprites()->fox_a2.texture);

		free((void *) sprites());
	}
}

void render_texture(const Texture2D texture, int x, int y, const int anchor_type, const float scale) {
	x = get_x_offset(anchor_type, texture.w, x, scale);
	y = get_y_offset(anchor_type, texture.h, y, scale);

	SDL_Rect texture_rect = {
		.x = x,
		.y = y,
		.w = (int) (texture.w * scale),
		.h = (int) (texture.h * scale)
	};

	SDL_RenderCopy(core()->renderer, texture.texture, NULL, &texture_rect);
}
