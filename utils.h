#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "consts.h"

/**
* Checks if the first argument is true otherwise quits the program and prints the given message and the SDL_Error if there is one.
*/
void check_sdl_failure(const int condition, const char *message);

/**
* Returns the x offset based on the given anchor type.
*/
int get_x_offset(const int anchor_type, const int w, const int x, const float scale);

/**
* Returns the y offset based on the given anchor type.
*/
int get_y_offset(const int anchor_type, const int h, const int y, const float scale);
