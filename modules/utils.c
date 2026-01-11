#include "../headers/utils.h"

void check_sdl_failure(const int condition, const char *message) {
	if (condition) {
		SDL_Quit();
		printf("%s! SDL_Error: %s\n", message, SDL_GetError());

		exit(EXIT_FAILURE);
	}
}
