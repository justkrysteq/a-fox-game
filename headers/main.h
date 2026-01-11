#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "core.h"

typedef struct {
	bool running;
} State;

typedef struct {
	SDL_Color white;
} Colors;

typedef struct {
	int x, y;
} Direction;

typedef struct {
	float x, y;
	Direction direction;
} Player;
