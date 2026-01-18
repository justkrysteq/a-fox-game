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

/**
* Returns State struct with initial values.
*/
State init_state(void);

/**
* Handles SDL events and player input.
*/
void handle_sdl_events(SDL_Event *event, State *state, Player *player);

/**
* Sets initial values for game state and player.
*/
void reset_game(State *state, Player *player);

/**
* Sets player's attack action as in action and updates its start time.
*/
void trigger_attack(Player *player, Action *attack);

/**
* Updates the texture for the FPS text displayed in info box.
*/
void update_fps(int *fps, char *fps_text, Texture2D *fps_text_texture);

/**
* Updates the texture for the time text displayed in info box.
*/
void update_time(int *time_s, char *time_text, Texture2D *time_text_texture);

/**
* Calculates the delta time between frames and returns its value in seconds.
*/
double calculate_delta_time(Uint64 frame_time, Uint64 *prev_frame_time);

/**
* Handles player movement boundaries to prevent player from going outside of the playable area.
*/
void handle_player_movement_bounds(Player *player);

/**
* Calculates the camera offset based on player's position.
*/
void calculate_camera_offset(const Player player, int *offset_x);

/**
* Renders the floor tiles.
*/
void render_floor(const int offset_x);

/**
* Renders the player.
*/
void render_player(const Player player, const int offset_x);

/**
* Returns a Player struct with initial values.
*/
Player init_player(void);

/**
* Stops the player's attack actions if their time has passed.
*/
void handle_player_attack(Player *player, int frame_time);
