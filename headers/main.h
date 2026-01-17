#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "core.h"

typedef struct {
	bool running;
	int time_s;
	int offset_x;
} State;

typedef struct {
	int x, y;
} Direction;

typedef struct {
	int action_time;
	bool in_action;
} Action;

typedef struct {
	float x, y;
	Direction direction;
	Action attack_light;
	Action attack_heavy;
} Player;
