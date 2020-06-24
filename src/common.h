/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <GLES3/gl32.h>

enum
{
	RET_NO_ERR,
	RET_SDL_ERR,
	RET_GL_ERR
};

typedef struct
{
	unsigned short rings;
	unsigned short sectors;

	unsigned short amount_of_ships;
	unsigned short ships_initial_sector_offset;

	unsigned short max_color_transition_length;
	unsigned short min_color_transition_length;

	unsigned short max_tick_time;
	unsigned short min_tick_time;

	float speed_multiplier;

	float hole_probability;
} Settings;

int run_SDL(Settings settings);
int run_GL(Settings settings, SDL_Window *window);
int run_game(Settings settings, SDL_Window *window);

int report_GL_errors(char *context);
