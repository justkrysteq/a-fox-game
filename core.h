#pragma once

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
	Texture2D background;
	Texture2D floor;

	Texture2D fox_u;
	Texture2D fox_d;
	Texture2D fox_l;
	Texture2D fox_r;
	Texture2D fox_idle;
	Texture2D fox_a1;
	Texture2D fox_a2;
} Sprites;

/**
* Returns const pointer to Core struct with initialized window, renderer and font.
*/
const Core *core(void);

/**
* Frees Core struct and performs TTF_Quit().
*/
void free_core(void);

/**
* Initializes SDL and TTF.
*/
void init_sdl(void);

/**
* Creates a texture from a given text string and returns it.
*
* Note: The texture inside needs to be destroyed with SDL_DestroyTexture().
*/
Texture2D create_text_texture(const char *text, TTF_Font *font, const SDL_Color color);

/**
* Creates a texture from a given sprite path and returns it.
* 
* Note: The texture inside needs to be destroyed with SDL_DestroyTexture().
*/
Texture2D create_sprite(const char *sprite_path, int w, int h, const float scale);

/**
* Returns const pointer to Sprites struct with initialized textures.
*/
const Sprites *sprites(void);

/**
* Frees Sprites struct and perofmrs SDL_DestroyTexture() on all of its textures.
*/
void free_sprites(void);

/**
* Renders a texture at a given position with a given anchor type and scale.
*/
void render_texture(const Texture2D texture, int x, int y, const int anchor_type, const float scale);
