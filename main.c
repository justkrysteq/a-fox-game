#include "main.h"

const SDL_Color WHITE = {255, 255, 255, 255};

int main(void) {
	init_sdl();

	State state = init_state();

	SDL_Event event;

	Uint64 prev_frame_time = SDL_GetTicks64();

	int fps = 0;
	char fps_text[16], time_text[16];
	strcpy(fps_text, "FPS: ?");
	strcpy(time_text, "Time: 0");
	Uint64 fps_update_time = SDL_GetTicks64();

	Texture2D fps_text_texture = create_text_texture(fps_text, core()->font, WHITE);
	Texture2D time_text_texture = create_text_texture(time_text, core()->font, WHITE);
	Texture2D implemented_text_texture = create_text_texture(IMPLEMENTED_TEXT, core()->font, WHITE);

	Player player = init_player();

	while (state.running) {
		SDL_RenderClear(core()->renderer);

		Uint64 frame_time = SDL_GetTicks64();
		double delta_time = calculate_delta_time(frame_time, &prev_frame_time);

		if (frame_time - fps_update_time >= 1000) {
			update_fps(&fps, fps_text, &fps_text_texture);
			update_time(&state.time_s, time_text, &time_text_texture);

			fps_update_time = frame_time;
		}

		handle_sdl_events(&event, &state, &player);
		player.x += player.direction.x * PLAYER_SPEED * delta_time;
		player.y += player.direction.y * PLAYER_SPEED * delta_time;

		handle_player_movement_bounds(&player);
		calculate_camera_offset(player, &state.offset_x);
		handle_player_attack(&player, frame_time);

		render_texture(sprites()->background, 0, INFO_BOX_HEIGHT, ANCHOR_TYPE_TOP_LEFT, 1);
		render_floor(state.offset_x);

		render_texture(fps_text_texture, INFO_BOX_PADDING, INFO_BOX_PADDING, ANCHOR_TYPE_TOP_LEFT, 1);
		render_texture(time_text_texture, WINDOW_WIDTH - INFO_BOX_PADDING, INFO_BOX_PADDING, ANCHOR_TYPE_TOP_RIGHT, 1);
		render_texture(implemented_text_texture, WINDOW_WIDTH/2 - INFO_BOX_PADDING, INFO_BOX_PADDING, ANCHOR_TYPE_TOP_CENTER, 1);

		render_player(player, state.offset_x);

		SDL_RenderPresent(core()->renderer);

		fps++;
	}

	SDL_DestroyTexture(fps_text_texture.texture);
	SDL_DestroyTexture(time_text_texture.texture);
	SDL_DestroyTexture(implemented_text_texture.texture);

	free_core();
	free_sprites();

	SDL_Quit();

	return EXIT_SUCCESS;
}

State init_state(void) {
	State state = {
		.running = true,
		.time_s = 0
	};

	return state;
}

void handle_sdl_events(SDL_Event *event, State *state, Player *player) {
	while (SDL_PollEvent(event)) {
		if (event->type == SDL_QUIT) {
			state->running = false;
		} else if (event->type == SDL_KEYDOWN) {
			switch (event->key.keysym.sym) {
				case KEY_QUIT:
					state->running = false;

					break;
				case KEY_MOVE_UP:
					player->direction.y = UP_DIRECTION;

					break;
				case KEY_MOVE_DOWN:
					player->direction.y = DOWN_DIRECTION;

					break;
				case KEY_MOVE_LEFT:
					player->direction.x = LEFT_DIRECTION;

					break;
				case KEY_MOVE_RIGHT:
					player->direction.x = RIGHT_DIRECTION;

					break;
				case KEY_NEW_GAME:
					reset_game(state, player);

					break;
				case KEY_LIGHT_ATTACK:
					trigger_attack(player, &player->attack_light);

					break;
				case KEY_HEAVY_ATTACK:
					trigger_attack(player, &player->attack_heavy);

					break;
			}
		} else if (event->type == SDL_KEYUP) {
			switch (event->key.keysym.sym) {
				case KEY_MOVE_UP:
				case KEY_MOVE_DOWN:
					player->direction.y = 0;

					break;
				case KEY_MOVE_LEFT:
				case KEY_MOVE_RIGHT:
					player->direction.x = 0;

					break;
			}
		}
	}
}

void reset_game(State *state, Player *player) {
	state->time_s = 0;
	player->x = WINDOW_WIDTH/2 - sprites()->fox_idle.w/2;
	player->y = WINDOW_HEIGHT*0.75;
	player->direction.x = 0;
	player->direction.y = 0;
	state->offset_x = 0;
}

void trigger_attack(Player *player, Action *attack) {
	if (!player->attack_light.in_action && !player->attack_heavy.in_action) {
		attack->in_action = true;
		attack->action_time = SDL_GetTicks64();
	}
}

void update_fps(int *fps, char *fps_text, Texture2D *fps_text_texture) {
	sprintf(fps_text, "FPS: %d", *fps);

	*fps = 0;

	*fps_text_texture = create_text_texture(fps_text, core()->font, WHITE);
}

void update_time(int *time_s, char *time_text, Texture2D *time_text_texture) {
	(*time_s)++;

	if (*time_s < 60 && *time_s >= 0) {
		sprintf(time_text, "Time: %d", *time_s);
	} else if (*time_s >= 60 && *time_s < 3600) {
		if ((*time_s)%60 < 10) {
			sprintf(time_text, "Time: %d:0%d", (*time_s)/60, (*time_s)%60);
		} else {
			sprintf(time_text, "Time: %d:%d", (*time_s)/60, (*time_s)%60);
		}
	}

	*time_text_texture = create_text_texture(time_text, core()->font, WHITE);
}

double calculate_delta_time(Uint64 frame_time, Uint64 *prev_frame_time) {
	double delta_time = (double) (frame_time - *prev_frame_time) / 1000;
	*prev_frame_time = frame_time;

	return delta_time;
}

void handle_player_movement_bounds(Player *player) {
	if (player->x < 0) {
		player->x = 0;
	} else if (player->x > WINDOW_WIDTH*FLOOR_TILE_COUNT - sprites()->fox_idle.w) {
		player->x = WINDOW_WIDTH*FLOOR_TILE_COUNT - sprites()->fox_idle.w;
	}
	if (player->y > WINDOW_HEIGHT) {
		player->y = WINDOW_HEIGHT;
	} else if (player->y < WINDOW_HEIGHT - FLOOR_HEIGHT + sprites()->fox_idle.h/2) {
		player->y = WINDOW_HEIGHT - FLOOR_HEIGHT + sprites()->fox_idle.h/2;
	}
}

void calculate_camera_offset(const Player player, int *offset_x) {
	if (player.x + *offset_x+sprites()->fox_idle.w > WINDOW_WIDTH*0.75 && player.direction.x == RIGHT_DIRECTION) {
		*offset_x = -1 * (player.x - WINDOW_WIDTH*0.75+sprites()->fox_idle.w);

	} else if (player.x + *offset_x < WINDOW_WIDTH*0.25 && player.direction.x == LEFT_DIRECTION) {
		*offset_x = -1 * (player.x - WINDOW_WIDTH*0.25);
	}

	if (*offset_x > 0) {
		*offset_x = 0;
	} else if (*offset_x < -1 * WINDOW_WIDTH*(FLOOR_TILE_COUNT-1)) {
		*offset_x = -1 * WINDOW_WIDTH*(FLOOR_TILE_COUNT-1);
	}
}

void render_floor(const int offset_x) {
	for (int i = 0; i < FLOOR_TILE_COUNT; i++) {
		render_texture(sprites()->floor, i*sprites()->floor.w+offset_x, WINDOW_HEIGHT, ANCHOR_TYPE_BOTTOM_LEFT, 1);
	}
}

void render_player(const Player player, const int offset_x) {
	if (player.attack_light.in_action) {
		render_texture(sprites()->fox_a1, player.x+offset_x, player.y, ANCHOR_TYPE_BOTTOM_LEFT, 1);
	} else if (player.attack_heavy.in_action) {
		render_texture(sprites()->fox_a2, player.x+offset_x, player.y, ANCHOR_TYPE_BOTTOM_LEFT, 1);
	} else if (player.direction.y == UP_DIRECTION && player.direction.x == 0) {
		render_texture(sprites()->fox_u, player.x+offset_x, player.y, ANCHOR_TYPE_BOTTOM_LEFT, 1);
	} else if (player.direction.y == DOWN_DIRECTION && player.direction.x == 0) {
		render_texture(sprites()->fox_d, player.x+offset_x, player.y, ANCHOR_TYPE_BOTTOM_LEFT, 1);
	} else if (player.direction.x == LEFT_DIRECTION) {
		render_texture(sprites()->fox_l, player.x+offset_x, player.y, ANCHOR_TYPE_BOTTOM_LEFT, 1);
	} else if (player.direction.x == RIGHT_DIRECTION) {
		render_texture(sprites()->fox_r, player.x+offset_x, player.y, ANCHOR_TYPE_BOTTOM_LEFT, 1);
	} else {
		render_texture(sprites()->fox_idle, player.x+offset_x, player.y, ANCHOR_TYPE_BOTTOM_LEFT, 1);
	}
}

Player init_player(void) {
	Player player = {
		.x = WINDOW_WIDTH/2 - sprites()->fox_idle.w/2,
		.y = WINDOW_HEIGHT*0.75,

		.direction.x = 0,
		.direction.y = 0,

		.attack_light.action_time = 0,
		.attack_light.in_action = false,

		.attack_heavy.action_time = 0,
		.attack_heavy.in_action = false
	};

	return player;
}

void handle_player_attack(Player *player, int frame_time) {
	if (player->attack_light.in_action && frame_time - player->attack_light.action_time >= LIGHT_ATTACK_TIME) {
		player->attack_light.in_action = false;
		player->attack_light.action_time = 0;
	}

	if (player->attack_heavy.in_action && frame_time - player->attack_heavy.action_time >= HEAVY_ATTACK_TIME) {
		player->attack_heavy.in_action = false;
		player->attack_heavy.action_time = 0;
	}
}
