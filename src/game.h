/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <math.h>

#define LOC_WH 0
#define LOC_POS 1
#define LOC_TEX_AREA 3

#define FULL_ANGLE 2*M_PI

void load_nums();
void free_nums();

void render_time_and_distance(SDL_Surface *target, unsigned int time_ms, unsigned int distance);
