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

void trigger_attack(Player *player, Action *attack);

State init_state(void);

void handle_sdl_events(SDL_Event *event, State *state, Player *player);

void reset_game(State *state, Player *player);

void trigger_attack(Player *player, Action *attack);

void update_fps(int *fps, char *fps_text, Texture2D *fps_text_texture);

void update_time(int *time_s, char *time_text, Texture2D *time_text_texture);

double calculate_delta_time(Uint64 frame_time, Uint64 *prev_frame_time);

void handle_player_movement_bounds(Player *player);

void calculate_camera_offset(const Player player, int *offset_x);

void render_floor(const int offset_x);

void render_player(const Player player, const int offset_x);

Player init_player(void);

void handle_player_attack(Player *player, int frame_time);
