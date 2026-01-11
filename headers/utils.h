#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "consts.h"

void check_sdl_failure(const int condition, const char *message);

int get_x_offset(const int anchor_type, const int w, const int x, const float scale);

int get_y_offset(const int anchor_type, const int h, const int y, const float scale);
