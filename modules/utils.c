#include "../headers/utils.h"

void check_sdl_failure(const int condition, const char *message) {
	if (condition) {
		SDL_Quit();
		printf("%s! SDL_Error: %s\n", message, SDL_GetError());

		exit(EXIT_FAILURE);
	}
}

int get_x_offset(const int anchor_type, const int w, const int x, const float scale) {
	switch (anchor_type) {
		default:
		case ANCHOR_TYPE_TOP_LEFT:
		case ANCHOR_TYPE_BOTTOM_LEFT:
			return x;
		case ANCHOR_TYPE_TOP_CENTER:
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
		case ANCHOR_TYPE_TOP_CENTER:
		case ANCHOR_TYPE_TOP_LEFT:
			return y;
		case ANCHOR_TYPE_CENTER:
			return y - h*scale / 2;
		case ANCHOR_TYPE_BOTTOM_LEFT:
			return y - h*scale;
	}
}
