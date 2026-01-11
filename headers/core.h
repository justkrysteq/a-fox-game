#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "consts.h"
#include "utils.h"

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_Font *font;
} Core;

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

const Core *core(void);

void free_core(void);

void init_sdl(void);

Texture2D create_text_texture(const char *text, TTF_Font *font, const SDL_Color color);

void render_dynamic_text(const char *text, const SDL_Color color, const int anchor_type, int x, int y);

Texture2D create_sprite(const char *sprite_assets_path, int w, int h, const float scale);

const Sprites *sprites(void);

void free_sprites(void);

void render_texture(Texture2D texture, int x, int y, const int anchor_type, const float scale);
