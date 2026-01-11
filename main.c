#include "headers/main.h"

const Colors *colors(void) { // NOTE: Might be better to use global consts
	static Colors *colors = NULL;

	if (!colors) {
		colors = (Colors *) malloc(sizeof(Colors));

		colors->white = (SDL_Color) {255, 255, 255, 255};
	}

	return colors;
}

State init_state(void) {
	State state = {
		.running = true
	};

	return state;
}

void handle_sdl_events(SDL_Event *event, State *state, Player *player) {
	while (SDL_PollEvent(event)) {
		if (event->type == SDL_QUIT) {
			state->running = false;
		} else if (event->type == SDL_KEYDOWN) {
			switch (event->key.keysym.sym) {
				case SDLK_q:
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
				case KEY_LIGHT_ATTACK:
					break;
				case KEY_HEAVY_ATTACK:
					break;
			}
		} else if (event->type == SDL_KEYUP) {
			switch (event->key.keysym.sym) {
				case KEY_QUIT:
					state->running = false;

					break;
				case KEY_MOVE_UP:
				case KEY_MOVE_DOWN:
					player->direction.y = 0;

					break;
				case KEY_MOVE_LEFT:
				case KEY_MOVE_RIGHT:
					player->direction.x = 0;

					break;
				case KEY_LIGHT_ATTACK:
					break;
				case KEY_HEAVY_ATTACK:
					break;
			}
		}
	}
}

void update_fps(int *fps, char *fps_text, Texture2D *fps_text_texture) {
	sprintf(fps_text, "FPS: %d", *fps);

	*fps = 0;

	*fps_text_texture = create_text_texture(fps_text, core()->font, colors()->white);
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

	*time_text_texture = create_text_texture(time_text, core()->font, colors()->white);
}

double calculate_delta_time(Uint64 frame_time, Uint64 *prev_frame_time) {
	double delta_time = (double) (frame_time - *prev_frame_time) / 1000;
	*prev_frame_time = frame_time;

	return delta_time;
}

int main(void) {
	init_sdl();

	State state = init_state();



	SDL_Event event;

	Uint64 prev_frame_time = SDL_GetTicks64();

	int fps = 0, time_s = 0;
	char fps_text[16], time_text[16];
	strcpy(fps_text, "FPS: ?");
	strcpy(time_text, "Time: 0");
	Uint64 fps_update_time = SDL_GetTicks64();

	Texture2D fps_text_texture = create_text_texture(fps_text, core()->font, colors()->white);
	Texture2D time_text_texture = create_text_texture(time_text, core()->font, colors()->white);


	Player player = {
		.x = WINDOW_WIDTH/2,
		.y = WINDOW_HEIGHT/2,
		.direction.x = 0,
		.direction.y = 0
	};

	while (state.running) {

		SDL_RenderClear(core()->renderer);

		Uint64 frame_time = SDL_GetTicks64();
		double delta_time = calculate_delta_time(frame_time, &prev_frame_time);

		if (frame_time - fps_update_time >= 1000) {
			update_fps(&fps, fps_text, &fps_text_texture);
			update_time(&time_s, time_text, &time_text_texture);
			
			fps_update_time = frame_time;
		}

		handle_sdl_events(&event, &state, &player);
		player.x += player.direction.x * PLAYER_SPEED * delta_time;
		player.y += player.direction.y * PLAYER_SPEED * delta_time;

		render_texture(fps_text_texture, 10, 10, ANCHOR_TYPE_TOP_LEFT, 1);
		render_texture(time_text_texture, WINDOW_WIDTH - 10, 10, ANCHOR_TYPE_TOP_RIGHT, 1);

		render_texture(sprites()->floor, 0, WINDOW_HEIGHT, ANCHOR_TYPE_BOTTOM_LEFT, 1);
		render_texture(sprites()->player, player.x, player.y, ANCHOR_TYPE_CENTER, 1);

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
